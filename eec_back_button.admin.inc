<?php

/**
 * Page callback: EEC back button link settings form
 *
 */
function eec_back_button_settings_form($form, &$form_state) {
    $form['eec_default_url_back_button'] = array(
        '#type' => 'textfield',
        '#title' => t('Default url for the button link'),
        '#default_value' => variable_get('eec_default_url_back_button', '<front>'),
        '#description' => t('The default url for the button link as a fallback when there is no referer.'),
        '#required' => TRUE,
    );

    return system_settings_form($form);
}
