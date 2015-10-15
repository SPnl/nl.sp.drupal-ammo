<?php

function ammo_amendment_form($form, &$form_state, $form_action = NULL, $path_id = NULL)
{
  global $user;

  // Store destination before ajax call.
  ammo_set_destination($form_state);

  // Check if motion is set in path and preset form if so.
  if (!empty($path_id) && !empty($form_action) && empty($form_state['storage']['first_step'])) {
    switch ($form_action) {
    case 'edit':
      if (ammo_preset_form('amendment', $path_id, $form_state)) {
        $form_state['storage']['first_step'] = 5;
      }
      else {
        return;
      }
      break;
    case 'advice':
      if (ammo_admin_access()) {
        if (ammo_preset_form('amendment', $path_id, $form_state)) {
          $form_state['storage']['first_step'] = 7;
        }
        else {
          return;
        }
      }
      else {
        return;
      }
      break;
    case 'view':
      $form = ammo_view_entity_form('amendment', $path_id);
      return $form;
      break;
    }
  }

  // Always set first step.
  if (empty($form_state['storage']['first_step'])) {
    $form_state['storage']['first_step'] = 1;
  }

  // Set laste page depending on permissions.
  if (empty($form_state['storage']['last_step'])) {
    if (ammo_admin_access()) {
      $form_state['storage']['last_step'] = 7;
    }
    else {
      $form_state['storage']['last_step'] = 6;
    }
  }

  // If $form_state['step'] is not set, we set it to the first step.
  $form_state['step'] = isset($form_state['step']) ? $form_state['step'] : $form_state['storage']['first_step'];

  // Add a wrapper div that will be used by the Form API to update the form using AJAX
  $form['#prefix'] = '<div id="ajax_form_multistep_form">';
  $form['#suffix'] = '</div>';

  $form['title'] = array(
    '#type' => 'item',
    '#markup' => '<h2>Stap '.($form_state['step'] - $form_state['storage']['first_step'] + 1).' / '.($form_state['storage']['last_step'] - $form_state['storage']['first_step'] + 1).'</h2>',
  );

  // Depending on which step of the form we are on, we output different form fields
  switch($form_state['step'])
  {

  case 1:
    ammo_select_meeting_form('amendment', $form, $form_state);
    break;

  case 2:
    $documents = entity_load('document');
    $options = array();
    foreach ($documents as $document) {
      if ($document->meeting_id == $form_state['storage']['meeting']) {
        $options[$document->id] = $document->title;
      }
    }
    $form['document'] = array(
      '#type' => 'select',
      '#title' => 'Document',
      '#options' => $options,
      '#empty_option' => 'kies:',
      '#default_value' => ammo_default($form_state, 'document'),
      '#required' => true,
      '#attributes' => array('onchange' => "jQuery('#action_button').mousedown()"),
    );
    break;

  case 3:
    ammo_select_afdeling_form('amendment', $form, $form_state);
    break;

  case 4:
    ammo_select_ammo_form('amendment', $form, $form_state, $form_state['storage']['document']);
    break;

  case 5:
    ammo_select_owner_form('amendment', $form, $form_state);
    break;

  case 6:
    $form['type'] = array(
      '#type' => 'select',
      '#title' => 'Type amendement',
      '#options' => array('specific' => 'Wijziging', 'general' => 'Algemeen'),
      '#default_value' => ammo_default($form_state, 'type'),
      '#name' => 'type',
    );
    $form['general'] = array(
      '#type' => 'fieldset',
      '#states' => array(
        'visible' => array(
          ':input[name="type"]' => array('value' => 'general'),
        ),
      ),
    );
    $form['general']['general'] = array(
      '#type' => 'textarea',
      '#title' => 'Amendement',
      '#default_value' => ammo_default($form_state, 'general'),
    );
    $form['specific'] = array(
      '#type' => 'fieldset',
      '#states' => array(
        'visible' => array(
          ':input[name="type"]' => array('value' => 'specific'),
        ),
      ),
    );
    $form['specific']['chapter'] = array(
      '#type' => 'textfield',
      '#title' => 'Hoofdstuk nr.',
      '#default_value' => ammo_default($form_state, 'chapter'),
    );
    $form['specific']['page'] = array(
      '#type' => 'textfield',
      '#title' => 'Pagina nr.',
      '#default_value' => ammo_default($form_state, 'page'),
    );
    $form['specific']['line'] = array(
      '#type' => 'textfield',
      '#title' => 'Regelnr.',
      '#default_value' => ammo_default($form_state, 'line'),
    );
    $form['specific']['action'] = array(
      '#type' => 'select',
      '#title' => 'Soort amendement',
      '#name' => 'action',
      '#options' => ammo_amendment_actions(),
      '#empty_option' => 'kies:',
      '#default_value' => ammo_default($form_state, 'action'),
    );
    $form['specific']['current_text'] = array(
      '#type' => 'textarea',
      '#title' => 'Huidige tekst',
      '#default_value' => ammo_default($form_state, 'current_text'),
      '#states' => array(
        'visible' => array(
          array(
            ':input[name="action"]' => array('value' => 'replace'),
          ),
          array(
            ':input[name="action"]' => array('value' => 'delete'),
          ),
        ),
      ),
    );
    $form['specific']['new_text'] = array(
      '#type' => 'textarea',
      '#title' => 'Nieuwe tekst',
      '#default_value' => ammo_default($form_state, 'new_text'),
      '#states' => array(
        'visible' => array(
          array(
            ':input[name="action"]' => array('value' => 'add'),
          ),
          array(
            ':input[name="action"]' => array('value' => 'replace'),
          ),
        ),
      ),
    );
    $form['supplement'] = array(
      '#type' => 'textarea',
      '#title' => 'Toelichting',
      '#default_value' => ammo_default($form_state, 'supplement'),
    );
    break;

  case 7:
    $form['advice'] = array(
      '#type' => 'select',
      '#title' => 'Advies',
      '#name' => 'advice',
      '#options'=> ammo_amendment_advice(),
      '#empty_option' => 'kies:',
      '#default_value' => ammo_default($form_state, 'advice'),
    );

    switch ($form_state['storage']['type']) {
    case 'general':
      $form['original_general'] = array(
        '#type' => 'item',
        '#markup' => '<p><strong>Ingediende amendement tekst</strong><br/>' . check_plain($form_state['storage']['general']) . '</p>',
        '#states' => array(
          'visible' => array(
            ':input[name="advice"]' => array('value' => 'accept_amended'),
          ),
        ),
      );
      $form['advice_general'] = array(
        '#type' => 'textarea',
        '#title' => 'Advies amendement tekst',
        '#default_value' => ammo_default($form_state, 'advice_general'),
        '#states' => array(
          'visible' => array(
            ':input[name="advice"]' => array('value' => 'accept_amended'),
          ),
        ),
      );
      break;
    case 'specific':
      $form['texts'] = array(
        '#type' => 'fieldset',
        '#states' => array(
          'visible' => array(
            ':input[name="advice"]' => array('value' => 'accept_amended'),
          ),
        ),
      );
      $form['texts']['advice_action'] = array(
        '#type' => 'select',
        '#title' => 'Soort amendement',
        '#name' => 'advice_action',
        '#options' => ammo_amendment_actions(),
        '#empty_option' => 'kies:',
        '#default_value' => ammo_default($form_state, 'advice_action'),
      );

      $form['texts']['original_current_text'] = array(
        '#type' => 'item',
        '#markup' => '<p><strong>Te vervangen/schrappen tekst orgineel</strong><br/>' . check_plain($form_state['storage']['current_text']) . '</p>',
        '#states' => array(
          'visible' => array(
            array(
              ':input[name="advice_action"]' => array('value' => 'delete'),
            ),
            array(
              ':input[name="advice_action"]' => array('value' => 'replace'),
            ),
          ),
        ),
      );
      $form['texts']['advice_current_text'] = array(
        '#type' => 'textarea',
        '#title' => 'Te vervangen/schrappen tekst in advies',
        '#default_value' => ammo_default($form_state, 'advice_current_text'),
        '#states' => array(
          'visible' => array(
            array(
              ':input[name="advice_action"]' => array('value' => 'delete'),
            ),
            array(
              ':input[name="advice_action"]' => array('value' => 'replace'),
            ),
          ),
        ),
      );
      $form['texts']['original_new_text'] = array(
        '#type' => 'item',
        '#markup' => '<p><strong>Nieuwe tekst orgineel</strong><br/>' . check_plain($form_state['storage']['current_text']) . '</p>',
        '#states' => array(
          'visible' => array(
            array(
              ':input[name="advice_action"]' => array('value' => 'add'),
            ),
            array(
              ':input[name="advice_action"]' => array('value' => 'replace'),
            ),
          ),
        ),
      );
      $form['texts']['advice_new_text'] = array(
        '#type' => 'textarea',
        '#title' => 'Nieuwe tekst in advies',
        '#default_value' => ammo_default($form_state, 'advice_new_text'),
        '#states' => array(
          'visible' => array(
            array(
              ':input[name="advice_action"]' => array('value' => 'add'),
            ),
            array(
              ':input[name="advice_action"]' => array('value' => 'replace'),
            ),
          ),
        ),
      );
      break;
    }
    $form['advice_supplement'] = array(
      '#type' => 'textarea',
      '#title' => 'Toelichting',
      '#default_value' => ammo_default($form_state, 'advice_supplement'),
    );
    $form['state'] = array(
      '#type' => 'select',
      '#title' => 'Status',
      '#options' => ammo_amendment_states(),
      '#empty_option' => 'kies:',
      '#default_value' => ammo_default($form_state, 'state'),
    );
    break;
  }

  // Create a container for our buttons
  $form['buttons'] = array
    (
      '#type' => 'container',
    );
  // If we are on first step, we do not want a back button
  if($form_state['step'] > $form_state['storage']['first_step'])
  {
    $form['buttons']['back'] = array
      (
        '#type' => 'submit',
        '#value' => t('Back'),
        '#name' => 'back',
        '#limit_validation_errors' => array(),
        '#submit' => array('ammo_amendment_form_back_submit'),
        '#ajax' => array
        (
          'wrapper' => 'ajax_form_multistep_form',
          'callback' => 'ammo_amendment_form_ajax_callback',
        ),
        '#id' => 'back',
      );
  }
  // Create submit button.
  if($form_state['step'] !== $form_state['storage']['last_step'])
  {
    $button_text = 'Volgende';
  }
  else {
    $button_text = 'Indienen';
  }
  $form['buttons']['forward'] = array
    (
      '#type' => 'submit',
      '#value' => $button_text,
      '#name' => 'action_button',
      '#validate' => array('ammo_amendment_form_validate'),
      '#submit' => array('ammo_amendment_form_submit'),
      '#id' => 'action_button',
      '#ajax' => array
      (
        // We pass in the wrapper we created at the start of the form
        'wrapper' => 'ajax_form_multistep_form',
        // We pass a callback function we will use later to render the form for the user
        'callback' => 'ammo_amendment_form_ajax_callback',
      ),
    );
  return $form;
}

function ammo_amendment_form_validate($form, &$form_state)
{
  $values = $form_state['values'];
  $storage = $form_state['storage'];
  $trigger = $form_state['triggering_element']['#name'];
  $step = $form_state['step'];

  $backed = $values['backed'];

  // First we set up a switch for each of our three steps
  switch($step) {
  case "5":
    switch ($trigger) {
    case 'action_button':
      if (!$backed) {
        $amendment = entity_load_single('amendment', $storage['amendment']);
        $owners =  ammo_get_related_contacts($amendment->id, 'amendment', 'owner');
        // Get amendment owners.
        if(empty($owners))
        {
          form_set_error('add_owner', t('Selecteer minstens één contact.'));
        }
      }
      break;
    case 'add_owner':
      $amendment = entity_load_single('amendment', $storage['amendment']);
      // Check if already owner.
      $owners =  ammo_get_related_contacts($amendment->id, 'amendment', 'owner');
      if (array_key_exists($values['owner'], $owners)) {
        form_set_error('add_owner', 'Contact was reeds toegevoegd.');
      }
      // Check if owner is member.
      $members = ammo_get_afdeling_members($storage['branch_id']);
      if (!array_key_exists($values['owner'], $members)) {
        form_set_error('add_owner', 'Geen lid.');
      }
      break;
    }
    break;
  case 6:
    if ($values['type'] == 'specific') {
      if (!preg_match('/^[1-9][0-9]*$/', $values['chapter'])) {
        form_set_error('chapter', 'Hoofdstuk nr. moet een getal zijn.');
      }
      if (!preg_match('/^[1-9][0-9]*$/', $values['page'])) {
        form_set_error('page', 'Pagina nr. moet een getal zijn.');
      }
      if (!preg_match('/^[1-9][0-9]*$/', $values['line'])) {
        form_set_error('chapter', 'Regelnummer moet een getal zijn.');
      }
      if (empty($values['action'])) {
        form_set_error('action', 'Kies het soort amendement.');
      }
      if ($values['action'] == 'add' && empty($values['new_text'])) {
        form_set_error('new_text', 'Vul de nieuwe tekst in.');
      }
      if ($values['action'] == 'delete' && empty($values['current_text'])) {
        form_set_error('current_text', 'Vul de huidige tekst die verwijderd moet worden in.');
      }
      if ($values['action'] == 'replace' && (empty($values['current_text']))) {
        form_set_error('current_text', 'Vul de huidige tekst die gewijzigd moet worden in.');
      }
      if ($values['action'] == 'replace' && (empty($values['new_text']))) {
        form_set_error('new_text', 'Vul de nieuwe tekst die de oude moet vervangen in.');
      }
    }
    if ($values['type'] == 'general') {
      if (empty($values['general'])) {
        form_set_error('general', 'Vul de amendement tekst in.');
      }
    }
    break;
  }
}

function ammo_amendment_form_back_submit($form, &$form_state)
{
  // Go step back.
  $form_state['step']--;

  // Because this is a multi-step form, we always need to rebuild the form
  // on every step, even after submission. This allows our form to be completely
  // Ajaxified without ever requiring a page load.
  $form_state['rebuild'] = TRUE;
}

function ammo_amendment_form_submit($form, &$form_state)
{
  $values = $form_state['values'];
  $storage = $form_state['storage'];
  $trigger = $form_state['triggering_element']['#name'];
  $step = $form_state['step'];

  switch ($trigger) {
  case 'action_button':
    switch ($step) {
    case 1:
      $form_state['storage']['meeting'] = $values['meeting'];
      break;
    case 2:
      $form_state['storage']['document'] = $values['document'];
      break;
    case 3:
      $form_state['storage']['branch_id'] = $values['branch_id'];
      $form_state['storage']['branch_display_name'] = $form['branch_id']['#options'][$values['branch_id']];
      break;
    case 4:
      if (empty($values['amendment'])) {
        // Save new amendment.
        $form_state['storage']['state'] = 'concept';
        $mapping = array(
          'document_id' => 'document',
          'branch_id' => 'branch_id',
          'branch_display_name' => 'branch_display_name',
          'state' => 'state',
        );
        $new_amendment = ammo_store('amendment', NULL, $mapping, $form_state);
        $form_state['storage']['amendment'] = $new_amendment->id;
      }
      else {
        // Get and store values from existing amendment in form.
        ammo_populate_form('amendment', $values['amendment'], $form_state);
      }
      break;
    case 5:
      $mapping = array(
        'backed' => 'backed',
      );
      ammo_store('amendment', $storage['amendment'], $mapping, $form_state);
      break;
    case 6:
      if ($storage['state'] === 'concept') {
        $form_state['storage']['state'] = 'submitted';
        $form_state['values']['state'] = 'submitted';
      }
      $mapping = array(
        'type' => 'amendment_type',
        'chapter' => 'chapter',
        'type' => 'type',
        'general' => 'general',
        'page' => 'page',
        'line' => 'line',
        'action' => 'action',
        'current_text' => 'current_text',
        'new_text' => 'new_text',
        'supplement' => 'supplement',
        'state' => 'state',
      );
      ammo_store('amendment', $storage['amendment'], $mapping, $form_state);
      break;
    case 7:
      $mapping = array(
        'advice' => 'advice',
        'advice_general' => 'advice_general',
        'advice_supplement' => 'advice_supplement',
        'advice_action' => 'advice_action',
        'advice_current_text' => 'advice_current_text',
        'advice_new_text' => 'advice_new_text',
        'state' => 'state',
      );
      ammo_store('amendment', $storage['amendment'], $mapping, $form_state);
      break;
    }
    // Set next page, or finish form.
    if ($storage['last_step'] == $step) {
      // Redirect user if destination is set.
      ammo_ajax_redirect($form_state['storage']['destination']['destination']);
      // Reset form.
      $form_state['step'] = 1;
      $form_state['storage'] = array();
      $form_state['storage']['first_step'] = 1;
      // Redirect users to page they came from.
    }
    else {
      // Increase the step by one, to move on to the next step
      $form_state['step']++;
    }
    break;
  case 'add_owner':
    switch ($step) {
    case 5:
      $amendment = entity_load_single('amendment', $storage['amendment']);
      // Sla nieuwe owner op.
      $display_name = ammo_contact_get_display_name($values['owner']);
      $owner = ammo_save_entity('amendments_owners', array(
        'amendment_id' => $amendment->id,
        'contact_id' => $values['owner'],
        'contact_display_name' => $display_name,
      ));
      if ($owner) {
        drupal_set_message('Contact is toegevoegd.');
      }
      else {
        drupal_set_message('Er is een fout opgetreden.' , 'error');
      }
      break;
    }
    break;
  }

  // Check if delete button was pressed.
  $pattern = '/^del-([^-]+)-(\d+)$/';
  if (preg_match($pattern, $trigger, $matches)) {
    $result = entity_delete($matches[1], $matches[2]);
    if (FALSE === $result) {
      drupal_set_message('Verwijderen contact is mislukt.', 'error');
    }
    else {
      drupal_set_message('Contact is verwijderd');
    }
  }

  $form_state['rebuild'] = TRUE;
}

function ammo_amendment_form_ajax_callback($form, &$form_state)
{
  return $form;
}
