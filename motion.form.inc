<?php

function ammo_motion_form($form, &$form_state, $path_id = NULL)
{
  global $user;

  // Check if motion is set in path and preset form if so.
  if (!empty($path_id) && empty($form_state['storage']['first_step'])) {
    $form_state['storage']['first_step'] = 4;
    ammo_preset_form('motion', $path_id, $form_state);
  }

  // Always set first step.
  if (empty($form_state['storage']['first_step'])) {
    $form_state['storage']['first_step'] = 1;
  }

  // Set laste page depending on permissions.
  if (empty($form_state['storage']['last_step'])) {
    if (ammo_admin_access()) {
      $form_state['storage']['last_step'] = 6;
    }
    else {
      $form_state['storage']['last_step'] = 5;
    }
  }

  // If $form_state['step'] is not set, we set it to 1
  $form_state['step'] = isset($form_state['step']) ? $form_state['step'] : $form_state['storage']['first_step'];

  // Skip single selection forms.
  if ($form_state['step'] === 1) {
    $meetings = entity_load('meeting');
    if (count($meetings) === 1) {
      $form_state['storage']['meeting'] = $meetings[1]->id;
      $form_state['step'] = 2;
      drupal_set_message('Stap 1 is automatisch ingevuld.');
    }
  }
  if ($form_state['step'] === 2) {
    $afdelingen = ammo_afdelingen_access();
    if (count($afdelingen) === 1) {
      $form_state['storage']['afdeling'] = current(array_keys($afdelingen));
      $form_state['step'] = 3;
      drupal_set_message('Stap 2 is automatisch ingevuld.');
    }
  }

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
    ammo_select_afdeling_form('amendment', $form, $form_state);
    break;

  case 3:
    ammo_select_ammo_form('motion', $form, $form_state, $form_state['storage']['meeting']);
    break;

  case 4:
    ammo_select_owner_form('motion', $form, $form_state);
    break;

  case 5:
    $form['consideration_opening'] = array(
      '#type' => 'textfield',
      '#title' => 'Overweging openingszin',
      '#default_value' => ammo_default($form_state, 'consideration_opening'),
    );

    $form['consideration_text'] = array(
      '#type' => 'textarea',
      '#title' => 'Overweging inhoud',
      '#default_value' => ammo_default($form_state, 'consideration_text'),
    );

    $form['follow_up_opening'] = array(
      '#type' => 'textfield',
      '#title' => 'Vervolg openingszin',
      '#default_value' => ammo_default($form_state, 'follow_up_opening'),
    );

    $form['follow_up_text'] = array(
      '#type' => 'textarea',
      '#title' => 'Vervolg inhoud',
      '#default_value' => ammo_default($form_state, 'follow_up_text'),
    );

    $form['supplement'] = array(
      '#type' => 'textarea',
      '#title' => 'Toelichting',
      '#default_value' => ammo_default($form_state, 'supplement'),
    );
    break;

  case 6:
    $form['advice'] = array(
      '#type' => 'select',
      '#title' => 'Advies',
      '#name' => 'advice',
      '#options'=> ammo_motion_advice(),
      '#empty_option' => 'kies:',
      '#default_value' => ammo_default($form_state, 'advice'),
    );

    $form['texts'] = array(
      '#type' => 'fieldset',
      '#states' => array(
        'visible' => array(
          ':input[name="advice"]' => array('value' => 'accept_amended'),
        ),
      ),
    );

    $form['texts']['original_consideration_opening'] = array(
      '#type' => 'item',
      '#markup' => '<p><strong>Overweging openingszin origineel</strong><br/>' . check_plain($form_state['storage']['consideration_opening']),
    );
    $form['texts']['advice_consideration_opening'] = array(
      '#type' => 'textfield',
      '#title' => 'Overweging openingszin in advies',
      '#default_value' => ammo_default($form_state, 'advice_consideration_opening'),
    );

    $form['texts']['original_consideration_text'] = array(
      '#type' => 'item',
      '#markup' => '<p><strong>Overweging inhoud orgineel</strong><br/>' . check_plain($form_state['storage']['consideration_text']),
    );
    $form['texts']['advice_consideration_text'] = array(
      '#type' => 'textarea',
      '#title' => 'Overweging inhoud in advies',
      '#default_value' => ammo_default($form_state, 'advice_consideration_text'),
    );

    $form['texts']['original_follow_up_opening'] = array(
      '#type' => 'item',
      '#markup' => '<p><strong>Overweging inhoud orgineel</strong><br/>' . check_plain($form_state['storage']['follow_up_opening']),
    );
    $form['texts']['advice_follow_up_opening'] = array(
      '#type' => 'textfield',
      '#title' => 'Vervolg openingszin',
      '#default_value' => ammo_default($form_state, 'advice_follow_up_opening'),
    );

    $form['texts']['original_consideration_text'] = array(
      '#type' => 'item',
      '#markup' => '<p><strong>Overweging inhoud orgineel</strong><br/>' . check_plain($form_state['storage']['consideration_text']),
    );
    $form['texts']['advice_follow_up_text'] = array(
      '#type' => 'textarea',
      '#title' => 'Vervolg inhoud',
      '#default_value' => ammo_default($form_state, 'advice_follow_up_text'),
    );

    $form['advice_supplement'] = array(
      '#type' => 'textarea',
      '#title' => 'Toelichting',
      '#default_value' => ammo_default($form_state, 'advice_supplement'),
    );

    $form['state'] = array(
      '#type' => 'select',
      '#title' => 'Status',
      '#options' => ammo_motion_states(),
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
        '#submit' => array('ammo_motion_form_back_submit'),
        '#ajax' => array
        (
          'wrapper' => 'ajax_form_multistep_form',
          'callback' => 'ammo_motion_form_ajax_callback',
        ),
        '#id' => 'back',
      );
  }
  // Create submit button.
  if($form_state['step'] !== $form_state['storage']['last_step'])
  {
    $button_text = 'Volgend';
  }
  else {
    $button_text = 'Indienen';
  }
  $form['buttons']['forward'] = array
    (
      '#type' => 'submit',
      '#value' => $button_text,
      '#name' => 'action_button',
      '#validate' => array('ammo_motion_form_validate'),
      '#submit' => array('ammo_motion_form_submit'),
      '#id' => 'action_button',
      '#ajax' => array
      (
        // We pass in the wrapper we created at the start of the form
        'wrapper' => 'ajax_form_multistep_form',
        // We pass a callback function we will use later to render the form for the user
        'callback' => 'ammo_motion_form_ajax_callback',
      ),
    );
  return $form;
}

function ammo_motion_form_validate($form, &$form_state)
{
  $values = $form_state['values'];
  $storage = $form_state['storage'];
  $trigger = $form_state['triggering_element']['#name'];
  $step = $form_state['step'];

  $backed = $values['backed'];

  // First we set up a switch for each of our three steps
  switch($step)
  {
  case "4":
    switch ($trigger) {
    case 'action_button':
      if (!$backed) {
        $motion = entity_load_single('motion', $storage['motion']);
        $owners =  ammo_get_related_contacts($motion->id, 'motion', 'owner');
        // Get motion owners.
        if(empty($owners))
        {
          form_set_error('add_owner', t('Selecteer minstens één contact.'));
        }
      }
      break;
    case 'add_owner':
      $motion = entity_load_single('motion', $storage['motion']);
      // Check if already owner.
      $owners =  ammo_get_related_contacts($motion->id, 'motion', 'owner');
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
  }
}

function ammo_motion_form_back_submit($form, &$form_state)
{
  // Go step back.
  $form_state['step']--;

  // Because this is a multi-step form, we always need to rebuild the form
  // on every step, even after submission. This allows our form to be completely
  // Ajaxified without ever requiring a page load.
  $form_state['rebuild'] = TRUE;
}

function ammo_motion_form_submit($form, &$form_state)
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
      $form_state['storage']['branch_id'] = $values['branch_id'];
      break;
    case 3:
      if (empty($values['motion'])) {
        // Save new motion.
        $form_state['storage']['state'] = 'concept';
        $mapping = array(
          'meeting_id' => 'meeting',
          'branch_id' => 'branch_id',
          'state' => 'state',
        );
        $new_motion = ammo_store('motion', NULL, $mapping, $form_state);
        $form_state['storage']['motion'] = $new_motion->id;
      }
      else {
        // Get and store values from existing motion in form.
        ammo_populate_form('motion', $values['motion'], $form_state);
      }
      break;
    case 4:
      $mapping = array(
        'backed' => 'backed',
      );
      ammo_store('motion', $storage['motion'], $mapping, $form_state);
      break;
    case 5:
      if ($storage['state'] === 'concept') {
        $form_state['storage']['state'] = 'submitted';
        $form_state['values']['state'] = 'submitted';
      }
      $mapping = array(
        'consideration_opening' => 'consideration_opening',
        'consideration_body' => 'consideration_text',
        'follow_up_opening' => 'follow_up_opening',
        'follow_up_body' => 'follow_up_text',
        'supplement' => 'supplement',
        'state' => 'state',
      );
      ammo_store('motion', $storage['motion'], $mapping, $form_state);
      break;
    case 6:
      $mapping = array(
        'advice' => 'advice',
        'advice_supplement' => 'advice_supplement',
        'advice_consideration_opening' => 'advice_consideration_opening',
        'advice_consideration_body' => 'advice_consideration_text',
        'advice_follow_up_opening' => 'advice_follow_up_opening',
        'advice_follow_up_body' => 'advice_follow_up_text',
        'state' => 'state',
      );
      ammo_store('motion', $storage['motion'], $mapping, $form_state);
      break;
    }
    // Set next page, or finish form.
    if ($storage['last_step'] == $step) {
      // Reset form.
      $form_state['step'] = 1;
      $form_state['storage'] = array();
      $form_state['storage']['first_step'] = 1;
    }
    else {
      // Increase the step by one, to move on to the next step
      $form_state['step']++;
    }
    break;
  case 'add_owner':
    switch ($step) {
    case 4:
      $motion = entity_load_single('motion', $storage['motion']);
      // Sla nieuwe owner op.
      $owner = ammo_save_entity('motions_owners', array(
        'motion_id' => $motion->id,
        'contact_id' => $values['owner'],
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

function ammo_motion_form_ajax_callback($form, &$form_state)
{
  return $form;
}
