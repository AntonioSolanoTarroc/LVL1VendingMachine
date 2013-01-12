from generateApiJson import generate
import urllib2
import json
from API_KEY import STORE_API_PRIVATE_KEY, LVL1_SQL_USER, LVL1_SQL_PASSWORD
import MySQLdb as mdb

STORE_API_URL = "http://192.168.1.104/opencart/index.php?route=api/api"
LVL1_SQL_URL = "mysql.lvl1.org"

def get_product_list(order_id):
    api_request = {'command': 'getProductsByInvoice', 'data': str(order_id)}
    full_request = STORE_API_URL + "&" + generate(STORE_API_PRIVATE_KEY, api_request)
    http_request = urllib2.Request(url=full_request)
    try:
        response = urllib2.urlopen(http_request)
    except urllib2.URLError:
        return (-1,)

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
    try:
        response = urllib2.urlopen(http_request)
    except urllib2.URLError:
        return False

    json_response = response.read()
    return json.loads(json_response)['data']

def create_order(card_number, item_number):

    try:
        con = mdb.connect(LVL1_SQL_URL, LVL1_SQL_USER, LVL1_SQL_PASSWORD, 'lvl1_members')
        cur = con.cursor(mdb.cursors.DictCursor)
        count = cur.execute("SELECT `member`.`id`, `first_name`, `last_name`, `email_address` FROM `member`"
            "LEFT JOIN `email` ON `member`.`id` = `email`.`member_id` WHERE `member`.`is_active` = 1 AND `member`.`key_id` = %s LIMIT 1", card_number)
        if(count > 0):
            data = cur.fetchoneDict()
            firstname = data['first_name']
            lastname = data['last_name']
            email = data['email_address']
        else:
            return False
    except mdb.Error, e:
        print "Error %d: %s" % (e.args[0],e.args[1])
    finally:
        if con:
            con.close()


    slot = item_number
    api_request = {'command': 'createOrder', 'data': {'firstname': firstname, 'lastname': lastname, 'email': email, 'slot_number': slot}}

    full_request = STORE_API_URL + "&" + generate(STORE_API_PRIVATE_KEY, api_request)
    http_request = urllib2.Request(url=full_request)
    try:
        response = urllib2.urlopen(http_request)
    except urllib2.URLError:
        return False

    #response = urllib2.urlopen(http_request)

    json_response = response.read()
    return json.loads(json_response)['data']

if __name__ == '__main__':
    print create_order("05A0A92", 1)