<?php
class ControllerApiApi extends Controller {
  private $error = array();

    public function index() {

      $valid_data = json_decode($this->get_verified_data(htmlspecialchars_decode($this->request->get['data'])), TRUE);
      $return_value;
      switch ($valid_data['command']) {
        case 'getProductsByInvoice':
          $product_locations = $this->getProductsByInvoice($valid_data['data']);
          $return_value = $product_locations;
          break;
        case 'orderFulfilled':
          $return_value = $this->orderFulfilled($valid_data['data']);
          break;
        case 'createOrder':
          $return_value = $this->createOrder($valid_data['data']['firstname'], $valid_data['data']['lastname'], $valid_data['data']['email'], $valid_data['data']['slot_number']);
          break;        
        default:
           $return_value = "no command";
          break;
      }

      $this->response->jsonOutput(json_encode(
        array(
          "data" => $return_value
        )));
    }

    private function createOrder($firstname, $lastname, $email, $slot_number)
    {

      $product_id_query = $this->db->query("SELECT `product_id` FROM `product` WHERE `location` = " . $slot_number . " AND `status` != 0");
      if($product_id_query->num_rows == 1) {

        $slot_product_id = $product_id_query->row['product_id'];

        $this->load->model('catalog/product');

        $slot_product = $this->model_catalog_product->getProduct($slot_product_id);

        $item_cost = $slot_product['price'];

        $total_data = array(
          array(
            'code' => 'sub_total',
            'title' => 'Sub-Total',
            'text' => '$' . strval($item_cost),
            'value' => $item_cost,
            'sort_order' => 1
            ),
          array(
            'code' => 'total',
            'title' => 'Total',
            'text' => '$' . strval($item_cost),
            'value' => $item_cost,
            'sort_order' => 9
            )
          );
        $total = $item_cost;
        $taxes = array();

        $data = array();
        
        $data['invoice_prefix'] = $this->config->get('config_invoice_prefix');
        $data['store_id'] = $this->config->get('config_store_id');
        $data['store_name'] = $this->config->get('config_name');
        
        if ($data['store_id']) {
          $data['store_url'] = $this->config->get('config_url');    
        } else {
          $data['store_url'] = HTTP_SERVER; 
        }
        
        $data['customer_id'] = 0;
        $data['customer_group_id'] = 1;
        $data['firstname'] = $firstname;
        $data['lastname'] = $lastname;
        $data['email'] = $email;
        $data['telephone'] = 5555555555;
        $data['fax'] = '';
        
        $data['payment_firstname'] = $firstname;
        $data['payment_lastname'] = $lastname; 
        $data['payment_company'] = ''; 
        $data['payment_company_id'] = ''; 
        $data['payment_tax_id'] = ''; 
        $data['payment_address_1'] = '814 E. Broadway';
        $data['payment_address_2'] = '';
        $data['payment_city'] = 'Louisville';
        $data['payment_postcode'] = '40204';
        $data['payment_zone'] = 'Kentucky';
        $data['payment_zone_id'] = '3639';
        $data['payment_country'] = 'United States';
        $data['payment_country_id'] = '223';
        $data['payment_address_format'] = '{firstname} {lastname} {company} {address_1} {address_2} {city}, {zone} {postcode} {country}';
      
        $data['payment_method'] = 'Cash On Delivery';
        
        $data['payment_code'] = 'cod';
                  
        $data['shipping_firstname'] = '';
        $data['shipping_lastname'] = '';  
        $data['shipping_company'] = ''; 
        $data['shipping_address_1'] = '';
        $data['shipping_address_2'] = '';
        $data['shipping_city'] = '';
        $data['shipping_postcode'] = '';
        $data['shipping_zone'] = '';
        $data['shipping_zone_id'] = '';
        $data['shipping_country'] = '';
        $data['shipping_country_id'] = '';
        $data['shipping_address_format'] = '';
        $data['shipping_method'] = '';
        $data['shipping_code'] = '';
        
        $product_data = array();
      
        $this->cart->add($slot_product_id);

        foreach ($this->cart->getProducts() as $product) {
          $option_data = array();
    
          foreach ($product['option'] as $option) {
            if ($option['type'] != 'file') {
              $value = $option['option_value']; 
            } else {
              $value = $this->encryption->decrypt($option['option_value']);
            } 
            
            $option_data[] = array(
              'product_option_id'       => $option['product_option_id'],
              'product_option_value_id' => $option['product_option_value_id'],
              'option_id'               => $option['option_id'],
              'option_value_id'         => $option['option_value_id'],                   
              'name'                    => $option['name'],
              'value'                   => $value,
              'type'                    => $option['type']
            );          
          }
     
          $product_data[] = array(
            'product_id' => $product['product_id'],
            'name'       => $product['name'],
            'model'      => $product['model'],
            'option'     => $option_data,
            'download'   => $product['download'],
            'quantity'   => $product['quantity'],
            'subtract'   => $product['subtract'],
            'price'      => $product['price'],
            'total'      => $product['total'],
            'tax'        => 0,
            'reward'     => $product['reward']
          ); 
        }
              
        $data['products'] = $product_data;
        $data['vouchers'] = array();
        $data['totals'] = $total_data;
        $data['comment'] = '';
        $data['total'] = $total;
        
        $data['affiliate_id'] = 0;
        $data['commission'] = 0;
        
        $data['language_id'] = 1;
        $data['currency_id'] = 2;
        $data['currency_code'] = 'USD';
        $data['currency_value'] = 1.0;
        $data['ip'] = '255.255.255.255';
        
        $data['forwarded_ip'] = '';
        
        $data['user_agent'] = '';
        
        $data['accept_language'] = '';
              
        $this->load->model('checkout/order');
        
        $current_order  = $this->model_checkout_order->addOrder($data);

        $this->model_checkout_order->confirm($current_order['order_id'], 1);

        $this->cart->clear();

        return true;
      } else {
        return false;
      }
    }

    private function orderFulfilled($invoice_no)
    {
      $order_query = $this->db->query("SELECT * FROM `" . DB_PREFIX . "order` WHERE invoice_no = '" . (int)$invoice_no . "' AND order_status_id > '0' AND order_status_id != '5'");
      
      if($order_query->num_rows) {
        $order_id = $order_query->row['order_id'];

        //5 is the completed order status id
        $update_order_query = $this->db->query("UPDATE `order` SET order_status_id = '" . 5 . "', date_modified = NOW() WHERE order_id = '" . (int)$order_id . "'");  
        return true;
      } else {
        return false;
      }
      
    }

    private function getProductsByInvoice($invoice_no) {
      $order_query = $this->db->query("SELECT * FROM `" . DB_PREFIX . "order` WHERE invoice_no = '" . (int)$invoice_no . "' AND order_status_id > '0' AND order_status_id != '5'");

      if ($order_query->num_rows) {
        $this->load->model('account/order');

        $products = $this->model_account_order->getOrderProducts($order_query->row['order_id']);

        $product_ids = array();

        foreach ($products as $index => $product) {
          for ($i=0; $i < $product['quantity']; $i++) { 
             $product_ids[] = $product['product_id'];
          }
        }

        $this->load->model('catalog/product');

        $product_slots = array();

        foreach ($product_ids as $idx => $product_id) {
          $product = $this->model_catalog_product->getProduct($product_id); 
          if($product['location'] != '') {
            $product_slots[] = $product['location'];
          }
        }
        return $product_slots;
      }
    }

    private function get_verified_data($data) {
      $received_data = $data;
      $decoded_data = json_decode(stripslashes($received_data), TRUE);

      $message = json_encode($decoded_data['data']);
      $received_signature = $decoded_data['sig'];
      $private_key = API_PRIVATE_KEY;
      $computed_signature = base64_encode(hash_hmac('sha1', $message, $private_key, TRUE)); 

      if($computed_signature == $received_signature) {  
              return $message;
      }  
      else {  
              return null;
      }

    }
}


?>