from generateApiJson import generate
import urllib2
import json
from API_KEY import STORE_API_PRIVATE_KEY

STORE_API_URL = "http://192.168.1.104/opencart/index.php?route=api/api"

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