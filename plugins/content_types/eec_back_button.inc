<?php

$plugin = array(
  'single' => TRUE,
  'title' => t('Back button link'),
  'description' => t('This is a custom pane that renders a button link that goes to the previous page from a referer.'),
  'category' => t('Custom Panes'),
  'edit form' => 'eec_back_button_form',
  'render callback' => 'eec_back_button_render',
  'admin info' => 'eec_back_button_info',
  'defaults' => array(
    'text' => '',
    'url' => '',
    'allowed_pages' => '',
  ),
  'all contexts' => TRUE,
//  'title singular' => t('Back button'),
//  'title plural' => t('Back buttons'),
//  'title singular proper' => t('Back button'),
//  'title plural proper' => t('Back buttons'),
);

/**
 * Edit form.
 */
function eec_back_button_form($form, &$form_state) {
  $conf = $form_state['conf'];

  $form['link_text'] = array(
    '#type' => 'textfield',
    '#title' => t('Text button link'),
    '#description' => t('The text that will show in the button link'),
    '#default_value' => $conf['link_text'],
  );

  $form['link_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Overrides default url for the button link'),
    '#description' => t('The url that will link into the button link. Leave empty to use the default value from the variable in the module configuration page.'),
    '#default_value' => $conf['link_url'],
  );

  $form['allowed_pages'] = array(
    '#type' => 'textarea',
    '#title' => t('Overrides default allowed pages button'),
    '#description' => t('The allowed pages within to render the button link, use comma to separate the values'),
    '#default_value' => $conf['allowed_pages'],
  );

  return $form;
}

/**
 * Edit form submit function.
 */
function eec_back_button_form_submit(&$form, &$form_state) {
  foreach (array_keys($form_state['plugin']['defaults']) as $key) {
    if (isset($form_state['values'][$key])) {
      $form_state['conf'][$key] = $form_state['values'][$key];
    }
  }
}

/**
 * Render the panel.
 */
function eec_back_button_render($subtype, $conf, $args, $contexts) {
  // If there is no contexts, then exit.
  if (empty($contexts)) {
    return FALSE;
  }
  // Get full term object for the root term.
  $conf_text = ctools_context_keyword_substitute($conf['link_text'], array(), $contexts);
  $conf_url = ctools_context_keyword_substitute($conf['link_url'], array(), $contexts);
  $allowed_pages = ctools_context_keyword_substitute($conf['allowed_pages'], array(), $contexts);
  $allowed_pages = explode(',', $allowed_pages);

  $matches = 0;
  foreach ($allowed_pages as $url_allowed) {
    if (strpos($_SERVER['HTTP_REFERER'], $url_allowed) !== FALSE) {
      $matches++;
    }
  }
  // Default url.
  $url = (!empty($conf_url) && filter_var($conf_url, FILTER_VALIDATE_URL, FILTER_FLAG_QUERY_REQUIRED)) ? $conf_url : variable_get('eec_default_url_back_button', '<front>');
  $request_uri = request_uri();

  // If the referer is within the allowed url aliases then get the link from the referer.
  if ($matches) {
    $url = $_SERVER['HTTP_REFERER'];
    $_SESSION['EEC_BACK_BUTTON'][$request_uri] = $url;
  }
  elseif(isset($_SESSION['EEC_BACK_BUTTON'][$request_uri])) {
    $url = $_SESSION['EEC_BACK_BUTTON'][$request_uri];
  }

  // Render block.
  $block = new stdClass();

  // initial content is blank
  $block->title = ''; // This will be overridden by the user within the panel options.
  $block->content = l($conf_text, $url, array('attributes' => array('class' => 'back-button')));

  return $block;

}

/**
 * Admin info.
 */
function eec_back_button_info($subtype, $conf, $contexts) {
  if (!empty($conf)) {
    $content = '<p><b>Link text:</b> ' . $conf['link_text'] . '</p>';
    $content .= '<p><b>Default link url override:</b> ' . $conf['link_url'] . '</p>';
    $content .= '<p><b>Allowed pages:</b> ' . $conf['allowed_pages'] . '</p>';


    // Render block.
    $block = new stdClass();

    // initial content is blank
    $block->title = ''; // This will be overridden by the user within the panel options.
    $block->content = $content;

    return $block;
  }
}