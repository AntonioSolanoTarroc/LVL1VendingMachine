#include <Python.h>

#define PYTHONMODULE "apiTests"

int initPythonFunctions(void);
static int getFunction(PyObject *module, PyObject **returnFunction, const char * functionName);
int getProductList(int invoiceNumber, int * rProductIds, int max_size);
int markOrderComplete(int invoiceNumber);
int createOrder(int cardId, int slotNumber);

int main(int argc, char *argv[])
{

	Py_Initialize();
	
	initPythonFunctions();

	int ids[22] = {[0 ... 21] = 0}; //Probably not necessary
	int items = getProductList(111113, ids, 22);

	printf("Returned Array: [");
	for(int i = 0; i < items; i++)
	{
		printf("%d, ", ids[i]);
	}
	printf("]\n");

	int rv = markOrderComplete(111113);
	printf("Returned Mark Order: %d\n", rv);

	rv = 0;
	rv = createOrder(111113, 3);
	printf("Returned Create Order: %d\n", rv);

	Py_Finalize();
	return 0;

}

PyObject *apiModule, *functionGetProductsByInvoice;
PyObject *functionMarkOrderCompleteByInvoice, *functionCreateOrder;

int initPythonFunctions(void)
{
	//Change path so that local module can be loaded.
	PyObject *path, *pwd;
	path = PySys_GetObject("path");
	pwd = PyString_FromString(".");
	PyList_Insert(path, 0, pwd);
	Py_DECREF(pwd);

	apiModule = PyImport_Import(PyString_FromString(PYTHONMODULE));

	if(apiModule != NULL)
	{
		getFunction(apiModule, &functionGetProductsByInvoice, "get_product_list");
		getFunction(apiModule, &functionMarkOrderCompleteByInvoice, "mark_order_complete");
		getFunction(apiModule, &functionCreateOrder, "create_order");
	} else {
		PyErr_Print();
        fprintf(stderr, "Failed to load module\n");
        return 1;
	}

	return 0;

}

static int getFunction(PyObject *module, PyObject **returnFunction, const char * functionName)
{
	*returnFunction = PyObject_GetAttrString(module, functionName);
	Py_INCREF(*returnFunction);
	if(*returnFunction && PyCallable_Check(*returnFunction))
	{
		return 0;
	} else {
		if (PyErr_Occurred())
			PyErr_Print();
		fprintf(stderr, "Cannot find function %s\n", functionName);
		return 1;
	}


}

int getProductList(int invoiceNumber, int * rProductIds, int max_size)
{
	int rv = 0;
	PyObject *orderId, *productIdTuple;

	orderId = Py_BuildValue("(i)", invoiceNumber);
	if(!orderId)
	{
		Py_DECREF(orderId);
		fprintf(stderr, "Cannot Convert Order ID %d\n", invoiceNumber);
		return -1;
	}

	productIdTuple = PyObject_CallObject(functionGetProductsByInvoice, orderId);
	Py_DECREF(orderId);

	if(productIdTuple != NULL && PyTuple_Check(productIdTuple))
	{
		int tupleItems = (int)PyTuple_Size(productIdTuple);
		if(tupleItems <= max_size)
		{
			for(int i = 0; i < tupleItems; i++)
			{
				PyObject *tupleItem;
				tupleItem = PyTuple_GetItem(productIdTuple, i);
				if(PyInt_Check(tupleItem))
				{
					rProductIds[i] = (int)PyInt_AsLong(tupleItem);
				}
			}
			rv = tupleItems;
		} else {
			fprintf(stderr, "Too Many Items in Returned Tuple!\n");
			return -1;
		}
		Py_DECREF(productIdTuple);
	} else {
		Py_XDECREF(productIdTuple);
		PyErr_Print();
		fprintf(stderr, "Could Not Retreive Products\n");
		return -1;
	}

	return rv;

}

int markOrderComplete(int invoiceNumber)
{
	int rv = 0;

	PyObject *orderId, *pythonReturn;

	orderId = Py_BuildValue("(i)", invoiceNumber);
	if(!orderId)
	{
		Py_DECREF(orderId);
		fprintf(stderr, "Cannot Convert Order ID %d\n", invoiceNumber);
		return -1;
	}

	pythonReturn = PyObject_CallObject(functionMarkOrderCompleteByInvoice, orderId);
	Py_DECREF(orderId);

	if(pythonReturn && PyBool_Check(pythonReturn))
	{
		rv = (int)PyInt_AsLong(pythonReturn);
	} else {
		fprintf(stderr, "Return Type is not boolean!\n");
	}

	Py_XDECREF(pythonReturn);

	return rv;
}

int createOrder(int cardId, int slotNumber)
{

	int rv = 0;

	PyObject *args, *pythonReturn;

	args = Py_BuildValue("(i, i)", cardId, slotNumber);
	if(!args)
	{
		Py_DECREF(args);
		fprintf(stderr, "Cannot Convert CardId or Slot Number\n");
		return -1;
	}

	pythonReturn = PyObject_CallObject(functionCreateOrder, args);
	Py_DECREF(args);

	if(pythonReturn && PyBool_Check(pythonReturn))
	{
		rv = (int)PyInt_AsLong(pythonReturn);
	} else {
		fprintf(stderr, "Return type is not boolean!\n");
	}

	Py_XDECREF(pythonReturn);

	return rv;

}
