from generateApiJson import generate
import urllib2
import json

STORE_API_URL = "http://192.168.1.104/opencart/index.php?route=api/api"
STORE_API_PRIVATE_KEY = "F_cJg-36.._9-3_2-UM.540mSwz50__..-.qZ-98w77-7_2Lm91_lf_EmHLl_y-"

def get_product_list(order_id):
    api_request = {'command': 'getProductsByInvoice', 'data': str(order_id)}
    full_request = STORE_API_URL + "&" + generate(STORE_API_PRIVATE_KEY, api_request)
    http_request = urllib2.Request(url=full_request)
    response = urllib2.urlopen(http_request)

    json_response = response.read()

    response_data = json.loads(json_response)['data']

    if(response_data is None):
        return (-1,)
    else:
        return tuple(map(int, response_data))

def mark_order_complete(order_id):
    api_request = {'command': 'orderFulfilled', 'data': str(order_id)}
    full_request = STORE_API_URL + "&" + generate(STORE_API_PRIVATE_KEY, api_request)
    http_request = urllib2.Request(url=full_request)
    response = urllib2.urlopen(http_request)

    json_response = response.read()
    return json.loads(json_response)['data']

def create_order(card_number, item_number):
    firstname = "Test"
    lastname = "Python"
    email = "TestPython@example.com"
    slot = item_number
    api_request = {'command': 'createOrder', 'data': {'firstname': firstname, 'lastname': lastname, 'email': email, 'slot_number': slot}}

    full_request = STORE_API_URL + "&" + generate(STORE_API_PRIVATE_KEY, api_request)
    http_request = urllib2.Request(url=full_request)
    response = urllib2.urlopen(http_request)

    json_response = response.read()
    return json.loads(json_response)['data']

if __name__ == '__main__':
    print create_order(1238488483, 1)