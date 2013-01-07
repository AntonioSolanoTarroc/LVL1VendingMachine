import hmac
import hashlib
import base64
import json
import urllib

def generate(privateKey, data):
    signature = hmac.new(privateKey, json.dumps(data, separators=(',', ':')), hashlib.sha1)
    jsonOutput = json.dumps({'sig': base64.b64encode(signature.digest()),
        'data': data}, separators=(',', ':'))

    return urllib.urlencode({'data': jsonOutput})

if __name__ == '__main__':
    print generate("F_cJg-36.._9-3_2-UM.540mSwz50__..-.qZ-98w77-7_2Lm91_lf_EmHLl_y-", {'data': {'firstname': 'Brad', 'lastname': 'Luyster1', 'email': 'test@example.com', 'slot_number': 1}, 'command': 'createOrder'})