<?php

function ammo_amendment_form($form, &$form_state, $form_action = NULL, $path_id = NULL)
{
  global $user;

  // Store destination before ajax call.
  ammo_set_destination($form_state);

  // Check if amendment is set in path and preset form if so.
  if (!empty($path_id) && !empty($form_action) && empty($form_state['storage']['first_step'])) {
    switch ($form_action) {
    case 'edit':
      if (ammo_populate_form('amendment', $path_id, $form_state)) {
        $meeting_id = ammo_get_meeting_id('amendment', $form_state['storage']['amendment']);
        // Check access right.
        if (
          $meeting_id &&
          isset($form_state['storage']['branch_id']) &&
          ammo_edit_access($meeting_id, $form_state['storage']['branch_id'])
        ) {
          $form_state['storage']['first_step'] = 5;
        }
        else {
          return;
        }
      }
      else {
        return;
      }
      break;
    case 'advice':
      if (ammo_admin_access()) {
        if (ammo_populate_form('amendment', $path_id, $form_state)) {
          $meeting_id = ammo_get_meeting_id('amendment', $form_state['storage']['amendment']);
          if (ammo_support_access($meeting_id) || ammo_superadmin_access()) {
            $form_state['storage']['first_step'] = 7;
          }
          else {
            return;
          }
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
  else {
    $meetings = ammo_get_meetings();
    if (empty($meetings)) { 
      drupal_set_message('De termijn voor het indienen van amendementen is verstreken.', 'warning');
      return;
    }
  }

  // Always set first step.
  if (empty($form_state['storage']['first_step'])) {
    $form_state['storage']['first_step'] = 1;
  }

  // Set data input step for changes check.
  $form_state['storage']['changes_step'] = 6;

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
  );

  // Depending on which step of the form we are on, we output different form fields
  if ($form_state['step'] == 1) {
    $form['title']['#markup'] = '<h2>Amendement indienen</h2>';
    ammo_checked_form($form, $form_state, 'amendment');
  }

  if ($form_state['step'] == 2) {
    $form['title']['#markup'] = '<h2>Amendement indienen</h2>';
    $options = ammo_get_meetings();
    if (count($options) == 1) {
      $form_state['step'] ++;
      $form_state['storage']['meeting'] = key($options);
      $form_state['storage']['meeting_display_name'] = current($options);
    }
    else {
      ammo_select_meeting_form($form, $form_state, $options);
    }
  }

  if ($form_state['step'] == 3) {
    $form['title']['#markup'] = '<h2>Amendement indienen</h2>';
    $options = ammo_get_documents($form_state);
    if (count($options) == 1) {
      $form_state['step'] ++;
      $form_state['storage']['document'] = key($options);
      $form_state['storage']['document_display_name'] = current($options);
    }
    else {
      $form['document'] = array(
        '#type' => 'select',
        '#title' => 'Document',
        '#description' => 'Kies het document wat u wilt amenderen.',
        '#options' => $options,
        '#default_value' => ammo_default($form_state, 'document'),
        '#required' => true,
        '#attributes' => array('onchange' => "jQuery('#action_button').mousedown()"),
        '#prefix' => '<p>Bijeenkomst: ' . check_plain($form_state['storage']['meeting_display_name']) . '</p>',
      );
    }
  }

  if ($form_state['step'] == 4) {
    $form['title']['#markup'] = '<h2>Amendement indienen</h2>';
    $options = ammo_afdelingen_access();
    if (empty($options)) {
      $options = array();
    }
    if (count($options) == 1) {
      $form_state['step'] ++;
      $form_state['storage']['branch_id'] = key($options);
      $form_state['storage']['branch_display_name'] = current($options);
      // Save new amendment.
      $form_state['storage']['state'] = 'concept';
      $mapping = array(
        'document_id' => 'document',
        'state' => 'state',
      );
      $new_amendment = ammo_store('amendment', NULL, $mapping, $form, $form_state);
      $form_state['storage']['amendment'] = $new_amendment->id;
      $form_state['storage']['new'] = TRUE;
    }
    else {
      ammo_select_afdeling_form($form, $form_state, 'amendment', $options);
      $form['branch_id']['#prefix'] =
        '<p>Bijeenkomst: ' . check_plain($form_state['storage']['meeting_display_name']) . '<br/>' .
        'Document: ' . check_plain($form_state['storage']['document_display_name'])  . '</p>';
    }
  }
  
  if ($form_state['step'] == 5) {
    $form['title']['#markup'] = '<h2>Amendement bewerken</h2>';
    ammo_select_owner_form('amendment', $form, $form_state);
    $form['backed']['#prefix'] =
      '<p>Bijeenkomst: ' . check_plain($form_state['storage']['meeting_display_name']) . '<br/>' .
      'Document: ' . check_plain($form_state['storage']['document_display_name'])  . '</p>'; 
  }

  if ($form_state['step'] == 6) {
    $form['title']['#markup'] = '<h2>Amendement bewerken</h2>';
    $form['chapter'] = array(
      '#type' => 'textfield',
      '#title' => 'Hoofdstuk nr.',
      '#default_value' => ammo_default($form_state, 'chapter'),
    );
    $form['page'] = array(
      '#type' => 'textfield',
      '#title' => 'Pagina nr.',
      '#default_value' => ammo_default($form_state, 'page'),
    );
    $form['line'] = array(
      '#type' => 'textfield',
      '#title' => 'Regelnr.',
      '#description' => 'Het regelnummer waar de bestaande tekst die geamendeerd wordt begint, of het regelnummer waar tekst moet worden toegevoegd.',
      '#default_value' => ammo_default($form_state, 'line'),
    );
    $form['amendment_text'] = array(
      '#type' => 'textarea',
      '#title' => 'Tekst amendement',
      '#default_value' => ammo_default($form_state, 'amendment_text'),
    );
    $form['supplement'] = array(
      '#type' => 'textarea',
      '#title' => 'Toelichting',
      '#default_value' => ammo_default($form_state, 'supplement'),
    );
  }

  if ($form_state['step'] == 7) {
    $form['title']['#markup'] = '<h2>Amendement advies</h2>';
    $form['item'] = array(
      '#type' => 'item',
      '#markup' => theme('amendment', array('entity_id' => $form_state['storage']['amendment'], 'no_links' => TRUE)),
    );

    $form['advice'] = array(
      '#type' => 'select',
      '#title' => 'Advies',
      '#name' => 'advice',
      '#options'=> ammo_amendment_advice(),
      '#empty_option' => 'kies:',
      '#default_value' => ammo_default($form_state, 'advice'),
    );
    $form['advice_supplement'] = array(
      '#type' => 'textarea',
      '#title' => 'Toelichting',
      '#default_value' => ammo_default($form_state, 'advice_supplement'),
    );
    $form['state'] = array(
      '#type' => 'select',
      '#title' => 'Status',
      '#options' => ammo_states(),
      '#empty_option' => 'kies:',
      '#default_value' => ammo_default($form_state, 'state'),
    );
  }

  // Create a container for our buttons
  $form['buttons'] = array
    (
      '#type' => 'container',
    );
  // If we are on first step, we do not want a back button
  if(
    $form_state['step'] > $form_state['storage']['first_step'] &&
    $form_state['step'] <> 5 &&
    !empty($form_state['active_steps'])
  )
  {
    $form['buttons']['back'] = array
      (
        '#type' => 'submit',
        '#value' => 'Terug',
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
  case 5:
    switch ($trigger) {
    case 'action_button':
      if (!$backed) {
        $amendment = entity_load_single('amendment', $storage['amendment']);
        $owners =  ammo_get_related_contacts($amendment->id, 'amendment', 'owner', 'member');
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
      $owners =  ammo_get_related_contacts($amendment->id, 'amendment', 'owner', 'member');
      if (array_key_exists($values['owner'], $owners)) {
        form_set_error('add_owner', 'Contact was reeds toegevoegd.');
      }
      // Check if owner is member.

      $admin_access = ammo_admin_access();
      if ($admin_access) {
        $members = ammo_get_all_members();
      }
      else {
        $members = ammo_get_afdeling_members($storage['branch_id']);
      }
      if (!array_key_exists($values['owner'], $members)) {
        form_set_error('add_owner', 'Contact bestaat niet.');
      }
      break;
    }
    break;
  case 6:
    if (!preg_match('/^[1-9][0-9]*$/', $values['chapter'])) {
      form_set_error('chapter', 'Hoofdstuk nr. moet een getal zijn.');
    }
    if (!preg_match('/^[1-9][0-9]*$/', $values['page'])) {
      form_set_error('page', 'Pagina nr. moet een getal zijn.');
    }
    if (!preg_match('/^[1-9][0-9]*$/', $values['line'])) {
      form_set_error('line', 'Regelnummer moet een getal zijn.');
    }
    // Extra validation for specific document!
    if ($storage['document'] == 2) {
      if ($values['chapter'] < 4) {
        form_set_error('chapter', 'De eerste drie hoofdstukken zijn niet amendeerbaar.');
      }
      if ($values['page'] < 10) {
        form_set_error('page', 'Controleer het paginanummer, alleen hoofdstuk 4 en verder zijn amendeerbaar.');
      }
    }
    if (empty($values['amendment_text'])) {
      form_set_error('amendment_text', 'Vul de amendement tekst in.');
    }
    break;
  }
}

function ammo_amendment_form_back_submit($form, &$form_state)
{
  // Go step back.
  $form_state['step'] = array_pop($form_state['active_steps']);

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
    case 2:
      $form_state['storage']['meeting'] = $values['meeting'];
      $form_state['storage']['meeting_display_name'] = $form['meeting']['#options'][$values['meeting']];
      break;
    case 3:
      $form_state['storage']['document'] = $values['document'];
      $form_state['storage']['document_display_name'] = $form['document']['#options'][$values['document']];
      break;
    case 4:
      $form_state['storage']['branch_id'] = $values['branch_id'];
      $form_state['storage']['branch_display_name'] = $form['branch_id']['#options'][$values['branch_id']];
      // Save new amendment.
      $form_state['storage']['state'] = 'concept';
      $mapping = array(
        'document_id' => 'document',
        'state' => 'state',
      );
      $new_amendment = ammo_store('amendment', NULL, $mapping, $form, $form_state);
      $form_state['storage']['amendment'] = $new_amendment->id;
      $form_state['storage']['new'] = TRUE;
      break;
    case 5:
      if ($values['backed']) {
        // Check if already set owner.
        if (!ammo_is_related($storage['amendment'], 'amendment', 'owner', 'branch', $storage['branch_id'])) {
          // Set owner.
          $entity = ammo_save_relation('owner', 'amendment', $storage['amendment'], $storage['branch_id'], 'branch', $storage['branch_display_name'], $storage['branch_id'], $storage['branch_display_name']);
        }
      }
      $form_state['storage']['new'] = FALSE;
      break;
    case 6:
      if ($storage['state'] === 'concept') {
        $form_state['storage']['state'] = 'submitted';
        $form_state['values']['state'] = 'submitted';
      }
      $mapping = array(
        'chapter' => 'chapter',
        'page' => 'page',
        'line' => 'line',
        'amendment_text' => 'amendment_text',
        'supplement' => 'supplement',
        'state' => 'state',
      );
      $amendment = ammo_store('amendment', $storage['amendment'], $mapping, $form, $form_state);
      // Reset storage variable.
      $values = $form_state['values'];
      $storage = $form_state['storage'];
      // Store chapterized id when not set yet.
      $query = 'SELECT MAX(chapterized_id) FROM {ammo_amendments} WHERE chapter = :chapter';
      $result = db_query($query, array(':chapter' => $storage['chapter'])); 
      $new_chapterized_id = $result->fetchField() + 1; 
      $amendment->chapterized_id = $new_chapterized_id;
      entity_save('amendment', $amendment);
      // Create short url.
      global $base_url;
      $long_url = $base_url . '/ammo/support/add/member/amendment/' . $storage['amendment'];
      $custom = 'am' . $storage['chapter'] . $new_chapterized_id;
      shurly_shorten($long_url, $custom);
      drupal_set_message('<h3>Opgeslagen</h3>' . theme('amendment', array('entity_id' => $storage['amendment'], 'no_links' => TRUE)));
      break;
    case 7:
      $mapping = array(
        'advice' => 'advice',
        'advice_supplement' => 'advice_supplement',
        'state' => 'state',
      );
      ammo_store('amendment', $storage['amendment'], $mapping, $form, $form_state);
      drupal_set_message('Het advies is opgeslagen.');
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
      $form_state['active_steps'][$form_state['step']] = $form_state['step'];
      $form_state['step']++;
    }
    break;
  case 'add_owner':
    switch ($step) {
    case 5:
      $form_state['storage']['new'] = FALSE;
      $amendment = entity_load_single('amendment', $storage['amendment']);
      // Sla nieuwe owner op.
      $display_name = ammo_contact_get_display_name($values['owner']);
      $owner = ammo_save_relation('owner', 'amendment', $amendment->id, $values['owner'], 'member', $display_name, $storage['branch_id'], $storage['branch_display_name']);
      if ($owner) {
        drupal_set_message('Contact is toegevoegd.');
        // Verwijder afdelingseigenaar.
        $contact_relation_id = ammo_is_related($storage['amendment'], 'amendment', 'owner', 'branch', $storage['branch_id']); 
        if ($contact_relation_id) {
          $result = entity_delete('contact_relation', $contact_relation_id);
          if (FALSE === $result) {
            drupal_set_message('Verwijderen contact is mislukt.', 'error');
          }
          else {
            drupal_set_message('Contact is verwijderd');
          }
        }
      }
      else {
        drupal_set_message('Er is een fout opgetreden.' , 'error');
      }
      break;
    }
    break;
  }

  // Check if delete button was pressed.
  $pattern = '/^del-(\d+)$/';
  if (preg_match($pattern, $trigger, $matches)) {
    $result = entity_delete('contact_relation', $matches[1]);
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
