<?php

/**
 *  Panel CTools create panel plugin
 */
$plugin = array(
  'single' => TRUE,
  'title' => t('HOD Product Display'),
  'description' => t('HOD Product Display'),
  'category' => t('Straight North'),
  'render callback' => 'hod_product_render',
  'admin info' => 'hod_product_listinfo',
  'defaults' => 'Test',
   'hook theme'    => array(
      'example_theme_pane' => array(
        'template' => 'hod_product-pane',
         'path' => drupal_get_path('module', 'hod_product') .'/plugins/styles',
         'variables' => array(
           'content' => NULL,
           'settings' => NULL,
        ),
      ),
    ),
  );


/**
 *  Render Panel
 */
function hod_product_render($subtype, $conf, $panel_args, $context = NULL) {
  $display = '';
  $node = node_load(arg(1));
  $price_code   = $node->field_price_code2['und'][0]['value'];
  $retail_price = $node->field_retail_price['und'][0]['value'];

  //$display = '<div class="price_code">Price Code: ' . $price_code . '</div>';

  $build_array =  array();
      // ATTRIBUTE RELATED NIDs
      if (count($node->field_product_attribute['und'])) {
        foreach ($node->field_product_attribute['und'] as $attribute_key => $attribute_value) {
          $entID = entity_load('field_collection_item', array($attribute_value['value']));
            $name  = $entID[$attribute_value['value']]->field_attribute_name['und'][0]['value'];
            $value = $entID[$attribute_value['value']]->field_attribute_value['und'][0]['value'];
            $nodes = $entID[$attribute_value['value']]->field_attribute_node['und'];


          $build_array[$name] = $value;
          if (count($nodes) > 0) {
            //dpm('name: '. $name);
            //dpm('value: ' . $value);
            //dpm('nodes');
            //dpm($nodes);
            foreach ($nodes as $key => $value) {
              $ref_node = node_load($value['nid']);
               if ($value['nid']) {
                 $returned_attribute[] = get_attribute($ref_node->nid, $node->nid, $name);
                 //dpm($returned_attribute);
               }
             }
           }
         }
       }

  $master_array = array();
  foreach ($build_array as $key => $row) {
    array_push($master_array, array($key => $row));
  }

  $i = 0;
  $items = array();
  foreach ($build_array as $key => $row) {
    foreach ($returned_attribute as $attrkey => $attrvalue) {
      //dpm($attrvalue);
      foreach($attrvalue as $attr_each_key => $attr_each_value) {
        if ($key == $attr_each_key) {
          $attr_nid = $attrvalue['nid'];
          $items[$key][$attr_each_value] = array($attr_nid => $attr_each_value);
        }
      }
    }
    $i++;
  }

  if (isset($attrvalue['none'])) {
    foreach($attrvalue['none'] as $attr_none_key => $attr_none_value) {
      //dpm('value');
      //dpm($attr_none_value);
      foreach ($attr_none_value as $none_key => $none_value) {
        //dpm('key:' . $none_key);
        //dpm('value: '. $none_value);
        //$items[$none_key] = $none_value;
        //$items['none'][$none_key] = $none_value;
      }

    }
  }

  foreach ($build_array as $key => $value) {
    $settitle[] = $value;
  }

  $display .= '<div class="prod-atrib-mix">
              <div class="pane-content">
              <div class="pane-node-field-product-attribute">
              <div class="pane-title">Details</div>
              <div class="view-content">';


  $display .= '<ul>';
    $i = 0;
    foreach ($build_array as $key => $value) {
      //$display .= '<li><strong><span class="item_key">' . $key . '</span></strong></li>';
      //$display .= '<li><span class="current_item">' . $value . '</span></li>';
    }
  $display .= '</ul>';

  $display .= '<ul>';
  $key_array = array();
  foreach ($items as $key => $value) {
    $li_class = strtolower(preg_replace('/ /', '', substr($key, 0, 5)));

    if (strlen($settitle[$i]) >0 ) {
      $key_array[] = $key;
      $display .= '<li><strong><span class="item_key">' . $key . '</span></strong></li>';
      $display .= '<li><span class="current_item ' . $li_class . '">' . $settitle[$i] . '</span></li>';
    }

    foreach ($value as $row => $content) {
      foreach ($content as $nid => $title) {
             $path = drupal_get_path_alias('node/' . $nid);
             $display .= '<li><span class="referenced_item ' . $li_class . '"><a href="/' . $path . '">' . $title . '</a></span></li>';
          }
        }
      $i++;
    }

  if (count($build_array) > 0) {
    foreach ($build_array as $key => $row) {
      if (!in_array($key, $key_array)) {
        $display .= '<li><strong><span class="item_key">' . $key . '</span></strong></li>';
        $display .= '<li><span class="detailtext current_item ' . $li_class . '">' . $row . '</span></li>';
      }
    }
  }

    $display .= '</ul>';
    if (strlen($price_code) > 0 ) {
      $display.= '<div class="price_code">Price Code: ' . $price_code . '</div>';
    }
    $display .= '</div></div></div></div>';

  $block = new stdClass();
  $block->content = $display;
  return $block;

}

function get_attribute($nid, $current_nid, $sentname) {

  //dpm('nid: '. $nid);
  //dpm('current nid: ' . $current_nid);
  //dpm('name: ' . $sentname);

  $attrnode = node_load($nid);
  $attribute_model = array();
  $attribute_temp = array();

    foreach ($attrnode->field_product_attribute['und'] as $attribute_key => $attribute_value) {
          $entIDattr = entity_load('field_collection_item', array($attribute_value['value']));

            foreach ($entIDattr[$attribute_value['value']]->field_attribute_node['und'] as $key => $value) {
              if ($value['nid'] == $current_nid) {
                //dpm('value nid: '. $value['nid']);
              }
            }

          $name  = $entIDattr[$attribute_value['value']]->field_attribute_name['und'][0]['value'];
          $value = $entIDattr[$attribute_value['value']]->field_attribute_value['und'][0]['value'];
          $refnid = $entIDattr[$attribute_value['value']]->field_attribute_node['und'][0]['nid'];


           if ($nid > 0) {
             //dpm('refnid: ' . $nid);
             //dpm('name: ' . $name);
             //dpm('value:' . $value);
             //dpm('-----');
           }


            //if ($attribute_key == 0) {
              //$attribute_model['nid'] = $nid;
            //}
        if ($sentname == $name) {
            $attribute_model['nid'] = $nid;
            $attribute_model[$name] = $value;
        }
         else {
            $attribute_model[none]['name'][$name] = $name;
            $attribute_model[none]['value'][$value] = $value;
         }
      }
    return $attribute_model;
  }
