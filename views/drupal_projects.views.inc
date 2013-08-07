<?php
/**
 * Implements hook_views_plugins().
 */
function drupal_projects_views_plugins() {
return array(
'style' => array(
  'rlist' => array(
    'title' => t('Grouped list'),
    'type' => 'normal',
    'path' => drupal_get_path('module', 'drupal_projecs') . '/views',
    'handler' => 'views_plugin_style_rlist',
    'uses fields' => TRUE,
    'uses row plugin' => TRUE,
    'uses options' => TRUE,
    'uses grouping' => TRUE,
    //'theme' => 'views_view_rlist',
    'theme' => 'views_view_unformatted',
    //'theme path' => drupal_get_path('module', 'drupal_projects') . '/views',
    //'theme file' => 'rlist.views.theme.inc',
    'help' => t('Render a view as a grouped list.'),
  )
));
}