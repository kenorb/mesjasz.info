<?php
// $Id: simplefeed_item.views.inc,v 1.1.2.2 2008/03/13 19:44:50 csevb10 Exp $

/**
 * Implementation of hook_views_data().
 */
function simplefeed_item_views_data() {
  $feed_item = array();
  $feed_item['table']['group'] = t('Simplefeed Feed Item');
  // Advertise this table as a possible base table
  $feed_item['table']['join']['node'] = array(
    'handler' => 'views_join',
    'arguments' => array('simplefeed_feed_item', 'node', 'vid', 'vid')
  );
  
  // title
  $feed_item['title'] = array(
    'title' => t('Article title'),
    'help' => t('The Title of the original page.'),
    'field' => array(
      'field' => 'title',
      'group' => t('Simplefeed Feed Item'),
      'handler' => 'views_handler_field_simplefeed_item_title',
    ),
  );
  
  // expires
  $feed_item['expires'] = array(
    'title' => t('Expires'),
    'help' => t('The default expiration date for a feed item.'),
    'field' => array(
      'field' => 'expires',
      'group' => t('Simplefeed Feed Item'),
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
      'help' => t('Sort by the default expiration date for a feed.'),
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
      'help' => t('Filter by the default expiration date for a feed. '),
    ),
  );
  
  // fid
  $feed_item['fid'] = array(
    'title' => t('Parent Feed Node ID'),
    'field' => array(
      'field' => 'fid',
      'group' => t('SimpleFeed Feed Item'),
      'handler' => 'views_handler_filter_numeric',
    ),
    // Information for accepting a fid as an argument
    'argument' => array(
      'handler' => 'views_handler_argument_node_nid',
      'click sortable' => TRUE,
      'help' => t('This argument will find all feed items for the specified feed node id.'),
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_simplefeed_parent',
      'help' => t('This allows you to filter feed items based on parent feed.'),
    ),
  );
  
  // url
  $feed_item['url'] = array(
    'title' => t('URL'),
    'help' => t('The URL to the original page for this feed item.'),
    'field' => array(
      'field' => 'url',
      'group' => t('Simplefeed Feed Item'),
      'handler' => 'views_handler_field_markup',
      'format' => 1,
    ),
  );
  
  $parent_node = array();
  $parent_node['table']['group'] = t('Simplefeed Feed Item');
  
  // Define the join
  $parent_node['table']['join']['node'] = array(
    'handler' => 'views_join',
    'arguments' => array('node', 'simplefeed_feed_item', 'fid', 'nid'),
  );
  
  // title
  $parent_node['title'] = array(
    'title' => t('Parent Feed Title'),
    'help' => t('Display the title of the parent feed node.'),
    'field' => array(
      'field' => 'title',
      'group' => t('Simplefeed Feed Item'),
      'handler' => 'views_handler_field_simplefeed_item_title',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
      'help' => t('Sort by the title of the parent feed node.'),
    ),
  );
  
  /*
  'fields' => array(
      'title' => array(
        'name' => t('SimpleFeed Feed Item: Parent Feed Title'),
        'handler' => array(
          'views_handler_field_simplefeed_parent_title'    => t('As Link'),
          'views_handler_field_simplefeed_parent_title_nl' => t('Without Link'),
        ),
        'addlfields' => array('nid'),
      ),
    ),
    'sorts' => array(
      'title' => array(
        'name' => t('SimpleFeed Feed Item: Parent Feed Title'),
        'help' => t('Sort by the title of the parent feed node.'),
      ),
    ),
  */
  
  return array('simplefeed_feed_item' => $feed_item, 'feed_parent_node' => $parent_node);
  $tables['simplefeed_feed_item'] = array(
    'fields' => array(
      'title' => array(
        'field' => 'url',
        'name' => t('SimpleFeed Feed Item: Article title'),
        'handler' => 'views_handler_field_simplefeed_item_title',
        'query_handler' => 'views_query_handler_field_simplefeed_item_title',
        'option' => array(
          '#type' => 'select',
          '#options' => array(
            1 => t('As link'),
            0 => t('Without link'),
          ),
        ),
        'help' => t('The Title of the original page.')
      )
    ),
    'filters' => array(
      'fid' => array(
        'name' => t('SimpleFeed Feed Item: Parent Feed Node ID'),
        'list' => 'views_handler_filter_simplefeed_parent',
        'operator' => 'views_handler_operator_andor',
        'help' => t('This allows you to filter feed items based on parent feed.'),
      ),
    )
  );

  // reference the node table using an alias table name
  $tables['feed_parent_node'] = array(
    'fields' => array(
      'title' => array(
        'name' => t('SimpleFeed Feed Item: Parent Feed Title'),
        'handler' => array(
          'views_handler_field_simplefeed_parent_title'    => t('As Link'),
          'views_handler_field_simplefeed_parent_title_nl' => t('Without Link'),
        ),
        'addlfields' => array('nid'),
        'help' => t('Display the title of the parent feed node.'),
      ),
    ),
    'sorts' => array(
      'title' => array(
        'name' => t('SimpleFeed Feed Item: Parent Feed Title'),
        'help' => t('Sort by the title of the parent feed node.'),
      ),
    ),
  );

  return $tables;
}

/**
 * Field handler to provide simple renderer that allows linking to a parent
 * feed.
 *
 * @ingroup views_field_handlers
 */
class views_handler_field_simplefeed_item_title extends views_handler_field_node {
  function init(&$view, &$data) {
    parent::init($view, $data);
    $this->additional_fields[] = 'nid'; 
    $this->nid_field = 'feed_parent_node_nid';
  }
  
  function render($values) {
    return $this->render_link(check_plain($values->{$this->field_alias}), $values);
  }
}

/**
 * Implementation of hook_views_default_views().
 */
function simplefeed_item_views_default_views() {
  // feeds overview page
  $view = new view();
  $view->name = 'feeds';
  $view->description = '';
  $view->tag = '';
  $view->view_php = '';
  $view->base_table = 'node';
  $view->is_cacheable = '0';
  $view->api_version = 2;
  $view->disabled = FALSE; // Edit this to true to make a default view disabled initially
  $view->display = array();  
  $display = new views_display();
  $display->id = 'default';
  $display->display_title = 'Defaults';
  $display->display_plugin = 'default';
  $display->position = '1';
  $display->display_options = array(
    'style_plugin' => 'default',
    'style_options' => array(),
    'row_plugin' => 'fields',
    'row_options' => array(),
    'relationships' => array(),
    'fields' => array(
      'title' => array(
        'id' => 'title',
        'table' => 'node',
        'field' => 'title',
        'link_to_node' => 1,
      ),
      'body' => array(
        'id' => 'body',
        'table' => 'node_revisions',
        'field' => 'body',
      ),
      'title_1' => array(
        'id' => 'title_1',
        'table' => 'feed_parent_node',
        'field' => 'title',
        'link_to_node' => 1,
      ),
    ),
    'sorts' => array(
      'created' => array(
        'id' => 'created',
        'table' => 'node',
        'field' => 'created',
        'order' => 'DESC',
      ),
    ),
    'arguments' => array(
      'fid' => array(
        'id' => 'fid',
        'table' => 'simplefeed_feed_item',
        'field' => 'fid',
        'default_action' => 'summary asc',
        'style_plugin' => 'default_summary',
        'style_options' => array(),
        'wildcard' => 'all',
        'wildcard_substitution' => 'All',
        'title' => '%1',
      ),
    ),
    'filters' => array(
      'type' => array(
        'id' => 'type',
        'table' => 'node',
        'field' => 'type',
        'operator' => 'in',
        'value' => array(
          'feed_item' => 'feed_item',
        ),
        'group' => 0,
        'expose_button' => array(
          'button' => 'Expose',
        ),
      ),
      'status' => array(
        'id' => 'status',
        'table' => 'node',
        'field' => 'status',
        'operator' => '=',
        'value' => 1,
        'group' => 0,
        'exposed' => false,
        'expose' => array(
          'operator' => false,
          'label' => '',
        ),
        'expose_button' => array(
          'button' => 'Expose',
        ),
      ),
    ),
    'items_per_page' => 10,
  );
  $view->display['default'] = $display;
  
  $display = new views_display();
  $display->id = 'page';
  $display->display_title = 'Page';
  $display->display_plugin = 'page';
  $display->position = '2';
  $display->display_options = array(
    'defaults' => array(
      'access' => true,
      'title' => true,
      'header' => true,
      'header_format' => true,
      'header_empty' => true,
      'footer' => true,
      'footer_format' => true,
      'footer_empty' => true,
      'empty' => true,
      'empty_format' => true,
      'items_per_page' => true,
      'offset' => true,
      'use_pager' => true,
      'pager_element' => true,
      'link_display' => true,
      'php_arg_code' => true,
      'exposed_options' => true,
      'style_plugin' => true,
      'style_options' => true,
      'row_plugin' => true,
      'row_options' => true,
      'relationships' => true,
      'fields' => true,
      'sorts' => true,
      'arguments' => true,
      'filters' => true,
    ),
    'relationships' => array(),
    'fields' => array(),
    'sorts' => array(),
    'arguments' => array(),
    'filters' => array(),
    'path' => 'feeds',
  );
  $view->display['page'] = $display;
  $views[$view->name] = $view;
 
  $view = new view();
  $view->name = 'feeds_latest';
  $view->description = 'Show the latest feeds items.';
  $view->tag = '';
  $view->view_php = '';
  $view->base_table = 'node';
  $view->is_cacheable = '0';
  $view->api_version = 2;
  $view->disabled = FALSE; // Edit this to true to make a default view disabled initially
  $view->display = array();
  $display = new views_display;
  $display->id = 'default';
  $display->display_title = 'Defaults';
  $display->display_plugin = 'default';
  $display->position = '1';
  $display->display_options = array(
    'style_plugin' => 'default',
    'style_options' => array(),
    'row_plugin' => 'fields',
    'row_options' => array(),
    'relationships' => array(),
    'fields' => array(
      'title' => array(
        'id' => 'title',
        'table' => 'node',
        'field' => 'title',
        'link_to_node' => 1,
      ),
      'teaser' => array(
        'id' => 'teaser',
        'table' => 'node_revisions',
        'field' => 'teaser',
        'label' => 'Node: Teaser',
      ),
      'title_1' => array(
        'id' => 'title_1',
        'table' => 'feed_parent_node',
        'field' => 'title',
        'link_to_node' => 1,
      ),
    ),
    'sorts' => array(
      'status' => array(
        'id' => 'status',
        'table' => 'node',
        'field' => 'status',
        'order' => 'DESC',
      ),
    ),
    'arguments' => array(),
    'filters' => array(
      'status' => array(
        'id' => 'status',
        'table' => 'node',
        'field' => 'status',
        'operator' => '=',
        'value' => 1,
        'group' => 0,
        'exposed' => false,
        'expose' => array(
          'operator' => false,
          'label' => '',
        ),
        'expose_button' => array('button' => 'Expose'),
      ),
      'type' => array(
        'id' => 'type',
        'table' => 'node',
        'field' => 'type',
        'operator' => 'in',
        'value' => array(
          'feed_item' => 'feed_item',
        ),
        'group' => 0,
        'exposed' => false,
        'expose' => array(
          'operator' => false,
          'label' => '',
        ),
        'expose_button' => array('button' => 'Expose'),
      ),
    ),
    'items_per_page' => 10,
    'use_pager' => '1',
    'pager_element' => 0,
    'title' => 'Latest feeds',
    'empty' => 'No feed items found.',
    'empty_format' => '1',
  );
  $view->display['default'] = $display;
  
  $display = new views_display();
  $display->id = 'page';
  $display->display_title = 'Page';
  $display->display_plugin = 'page';
  $display->position = '2';
  $display->display_options = array(
    'defaults' => array(
      'access' => true,
      'title' => true,
      'header' => true,
      'header_format' => true,
      'header_empty' => true,
      'footer' => true,
      'footer_format' => true,
      'footer_empty' => true,
      'empty' => true,
      'empty_format' => true,
      'items_per_page' => true,
      'offset' => true,
      'use_pager' => true,
      'pager_element' => true,
      'link_display' => true,
      'php_arg_code' => true,
      'exposed_options' => true,
      'style_plugin' => true,
      'style_options' => true,
      'row_plugin' => true,
      'row_options' => true,
      'relationships' => true,
      'fields' => true,
      'sorts' => true,
      'arguments' => true,
      'filters' => true,
    ),
    'relationships' => array(),
    'sorts' => array(),
    'arguments' => array(),
    'filters' => array(),
    'path' => 'feeds/latest',
  );
  $view->display['page'] = $display;
  
  $display = new views_display();
  $display->id = 'block';
  $display->display_title = 'Block';
  $display->display_plugin = 'block';
  $display->position = '3';
  $display->display_options = array(
    'defaults' => array(
      'access' => true,
      'title' => true,
      'header' => true,
      'header_format' => true,
      'header_empty' => true,
      'footer' => true,
      'footer_format' => true,
      'footer_empty' => true,
      'empty' => true,
      'empty_format' => true,
      'items_per_page' => true,
      'offset' => true,
      'use_pager' => true,
      'pager_element' => true,
      'link_display' => true,
      'php_arg_code' => true,
      'exposed_options' => true,
      'style_plugin' => false,
      'style_options' => false,
      'row_plugin' => false,
      'row_options' => false,
      'relationships' => true,
      'fields' => false,
      'sorts' => true,
      'arguments' => true,
      'filters' => true,
    ),
    'relationships' => array(),
    'fields' => array(
      'title' => array(
        'id' => 'title',
        'table' => 'node',
        'field' => 'title',
        'link_to_node' => 1,
      ),
      'title_1' => array(
        'id' => 'title_1',
        'table' => 'feed_parent_node',
        'field' => 'title',
        'link_to_node' => 1,
      ),
    ),
    'sorts' => array(),
    'arguments' => array(),
    'filters' => array(),
    'block_description' => 'Latest Feeds',
    'style_plugin' => 'list',
    'style_options' => array('type' => 'ul'),
    'row_plugin' => 'fields',
    'row_options' => array(),
  );
  $view->display['block'] = $display;
  
  $views[$view->name] = $view;
  return $views;
}