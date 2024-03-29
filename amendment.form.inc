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
    $meetings = ammo_get_meetings('submit');
    if (empty($meetings)) {
      drupal_set_message('De termijn voor het indienen van wijzigingsvoorstellen is verstreken.', 'warning');
      return;
    }
  }

  // Always set first step.
  if (empty($form_state['storage']['first_step'])) {
    $form_state['storage']['first_step'] = 1;
  }

  // Set laste page depending on permissions.
  $form_state['storage']['last_step'] = 6;
  if (ammo_admin_access()) {
    $form_state['storage']['last_step'] = 7;
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
    $form['title']['#markup'] = '<h2>Wijzigingsvoorstel indienen</h2>';
    ammo_checked_form($form, $form_state, 'amendment');
  }

  if ($form_state['step'] == 2) {
    $form['title']['#markup'] = '<h2>Wijzigingsvoorstel indienen</h2>';
    $options = ammo_get_meetings('submit');
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
    $form['title']['#markup'] = '<h2>Wijzigingsvoorstel indienen</h2>';
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
    $form['title']['#markup'] = '<h2>Wijzigingsvoorstel indienen</h2>';
    $meeting = entity_load_single('meeting', $form_state['storage']['meeting']);
    $options = ammo_afdelingen_access($meeting->admin_owner);
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
    $form['title']['#markup'] = '<h2>Wijzigingsvoorstel bewerken</h2>';
    ammo_select_owner_form('amendment', $form, $form_state);
    $form['backed']['#prefix'] =
      '<p>Bijeenkomst: ' . check_plain($form_state['storage']['meeting_display_name']) . '<br/>' .
      'Document: ' . check_plain($form_state['storage']['document_display_name'])  . '</p>';
  }

  if ($form_state['step'] == 6) {
    $form['title']['#markup'] = '<h2>Wijzigingsvoorstel bewerken</h2>';
    $form['chapter'] = array(
      '#type' => 'textfield',
      '#title' => 'Hoofdstuk nr.',
      '#default_value' => ammo_default($form_state, 'chapter'),
    );
    $form['page'] = array(
      '#type' => 'textfield',
      '#title' => 'Pagina nummer',
      '#default_value' => ammo_default($form_state, 'page'),
    );
    $form['line'] = array(
      '#type' => 'textfield',
      '#title' => 'Regel nummer (begin)',
      '#description' => 'Het regelnummer waar de tekst die geamendeerd wordt begint, of het regelnummer vanaf waar tekst moet worden toegevoegd.',
      '#default_value' => ammo_default($form_state, 'line'),
    );
    $form['amendment_text'] = array(
      '#type' => 'textarea',
      '#title' => 'Tekst wijzigingsvoorstel',
      '#default_value' => ammo_default($form_state, 'amendment_text'),
    );
    $form['supplement'] = array(
      '#type' => 'textarea',
      '#title' => 'Toelichting',
      '#default_value' => ammo_default($form_state, 'supplement'),
    );
  }

  if ($form_state['step'] == 7) {
    $form['title']['#markup'] = '<h2>Wijzigingsvoorstel advies</h2>';
    $form['item'] = array(
      '#type' => 'item',
      '#markup' => theme('amendment', array('entity_id' => $form_state['storage']['amendment'], 'no_links' => TRUE)),
    );

    $form['advice'] = array(
      '#type' => 'select',
      '#title' => 'Advies',
      '#name' => 'advice',
      '#options' => ammo_amendment_advice(),
      '#empty_option' => 'kies:',
      '#default_value' => ammo_default($form_state, 'advice'),
    );
    $form['advice_supplement'] = array(
      '#type' => 'textarea',
      '#title' => 'Toelichting advies',
      '#default_value' => ammo_default($form_state, 'advice_supplement'),
    );
    $form['state'] = array(
      '#type' => 'select',
      '#title' => 'Status',
      '#options' => ammo_states(),
      '#empty_option' => 'kies:',
      '#default_value' => ammo_default($form_state, 'state'),
    );
    $form['state_supplement'] = array(
      '#type' => 'textarea',
      '#title' => 'Toelichting statuswijziging',
      '#default_value' => ammo_default($form_state, 'state_supplement'),
    );
  }

  // Create a container for our buttons
  $form['buttons'] = array
    (
      '#type' => 'container',
    );
  // If we are on first step, we do not want a back button
  if (
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
  if ($form_state['step'] !== $form_state['storage']['last_step'])
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

function ammo_amendment_form_validate($form, &$form_state) {
  $values = $form_state['values'];
  $step = $form_state['step'];

  // First we set up a switch for each of our three steps
  switch($step) {
  case 6:
    if (!preg_match('/^[0-9][0-9]*$/', $values['chapter'])) {
      form_set_error('chapter', 'Hoofdstuk nr. moet een getal zijn.');
    }
    if (!preg_match('/^[0-9][0-9]*$/', $values['page'])) {
      form_set_error('page', 'Pagina nr. moet een getal zijn.');
    }
    if (!preg_match('/^[0-9][0-9]*$/', $values['line'])) {
      form_set_error('line', 'Regelnummer moet een getal zijn.');
    }
    if (empty($values['amendment_text'])) {
      form_set_error('amendment_text', 'Vul de wijzigingsvoorstel tekst in.');
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
          $type = 'branch';
          $display_name = $storage['branch_display_name'];
          $entity = ammo_save_relation('owner', 'amendment', $storage['amendment'], $storage['branch_id'], $type, $display_name, $storage['branch_id'], $storage['branch_display_name']);
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
      drupal_set_message('<h3>Opgeslagen</h3>' . theme('amendment', array('entity_id' => $storage['amendment'], 'no_links' => TRUE)));
      break;
    case 7:
      $mapping = array(
        'advice' => 'advice',
        'advice_supplement' => 'advice_supplement',
        'state' => 'state',
        'state_supplement' => 'state_supplement',
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
  }

  $form_state['rebuild'] = TRUE;
}

function ammo_amendment_form_ajax_callback($form, &$form_state)
{
  return $form;
}
