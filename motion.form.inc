<?php

function ammo_motion_form($form, &$form_state)
{
  global $user;

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
  $form_state['step'] = isset($form_state['step']) ? $form_state['step'] : 1;


  // Add a wrapper div that will be used by the Form API to update the form using AJAX
  $form['#prefix'] = '<div id="ajax_form_multistep_form">';
  $form['#suffix'] = '</div>';

  $form['title'] = array(
    '#type' => 'item',
    '#markup' => '<h2>Stap '.$form_state['step'].' / '.$form_state['storage']['last_step'].'</h2>',
  );

  // Depending on which step of the form we are on, we output different form fields
  switch($form_state['step'])
  {

  case 1:
    $meetings = entity_load('meeting');
    $options = array();
    foreach ($meetings as $meeting) {
      $options[$meeting->id] = $meeting->title;
    }
    $form['meeting'] = array(
      '#type' => 'select',
      '#title' => 'Bijeenkomst',
      '#options' => $options,
      '#empty_option' => 'kies:',
      '#default_value' => ammo_default($form_state, 'meeting'),
      '#required' => true,
      '#attributes' => array('onchange' => "jQuery('#action_button').mousedown()"),
    );
    break;

  case 2:
    // Afdelingen filteren op gebruikers id.
    //$afdelingen = ammo_get_afdelingen(61619);
    if(ammo_admin_access()) {
      $afdelingen = ammo_get_afdelingen();
    }
    else {
      $afdelingen = ammo_get_afdelingen($user->name);
    }
    $form['branch_id'] = array(
      '#type' => 'select',
      '#title' => 'Afdeling',
      '#options' => $afdelingen,
      '#empty_option' => 'kies:',
      '#default_value' => ammo_default($form_state, 'branch_id'),
      '#required' => true,
      '#attributes' => array('onchange' => "jQuery('#action_button').mousedown()"),
    );
    break;

  case 3:
    $motions = ammo_get_amendments_or_motions('motion', $form_state['storage']['branch_id'], $form_state['storage']['meeting']);
    $form['motion'] = array(
      '#type' => 'select',
      '#title' => 'Motie nr.',
      '#options' => $motions,
      '#empty_option' => 'nieuw',
      '#default_value' => ammo_default($form_state, 'motion'),
      '#attributes' => array('onchange' => "jQuery('#action_button').mousedown()"),
    );
    break;

  case 4:
    $form['owners'] = array(
      '#type' => 'fieldset',
    );
    $owners = ammo_get_related_contacts($form_state['storage']['motion'], 'motion', 'owner');
    // Show owners.
    $form['owners']['intro'] = array(
      '#type' => 'item',
      '#markup' => '<p><strong>Motie ingediend door:</strong></p>',
    );
    foreach ($owners as $owner_id => $data) {
      $form['owners']['del_owner_' . $owner_id] = array(
        '#type' => 'submit',
        '#value' => 'x',
        '#name' => 'del-motions_owners-' . $data['entity_id'],
        '#prefix' => '<p>',
        '#suffix' =>  $data['display_name'] . ' - ' . $owner_id . '</p> ',
        '#ajax' => array(
          'callback' => 'ammo_motion_form_ajax_callback',
          'wrapper' => 'ajax_form_multistep_form',
        ),
        '#attributes' => array('class' => array('ammo_owner_del_button')),
      );
    }
    // create add participant button.
    $form['owners']['owner'] = array(
      '#type' => 'textfield',
      '#title' => 'naam / lidnummer',
      '#autocomplete_path' => 'ammo/autocomplete_members/' . $form_state['storage']['branch_id'],
    );
    $form['owners']['add_owner'] = array(
      '#type' => 'submit',
      '#value' => 'Voeg toe',
      '#name' => 'add_owner',
      '#validate' => array('ammo_motion_form_validate'),
      '#submit' => array('ammo_motion_form_submit'),
      '#ajax' => array
      (
        'wrapper' => 'ajax_form_multistep_form',
        'callback' => 'ammo_motion_form_ajax_callback',
      ),
    );
    // Backed by local branch?
    $form['backed'] = array(
      '#type' => 'checkbox',
      '#title' => 'Motie wordt ondersteund door afdeling',
      '#default_value' => ammo_default($form_state, 'backed'),
    );
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

    $form['texts']['advice_consideration_opening'] = array(
      '#type' => 'textfield',
      '#title' => 'Overweging openingszin',
      '#default_value' => ammo_default($form_state, 'advice_consideration_opening'),
    );

    $form['texts']['advice_consideration_text'] = array(
      '#type' => 'textarea',
      '#title' => 'Overweging inhoud',
      '#default_value' => ammo_default($form_state, 'advice_consideration_text'),
    );

    $form['texts']['advice_follow_up_opening'] = array(
      '#type' => 'textfield',
      '#title' => 'Vervolg openingszin',
      '#default_value' => ammo_default($form_state, 'advice_follow_up_opening'),
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
  // If we are on step 1, we do not want a back button
  if($form_state['step'] !== 1)
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

  // First we set up a switch for each of our three steps
  switch($step)
  {
  case "4":
    switch ($trigger) {
    case 'action_button':
      $motion = entity_load_single('motion', $storage['motion']);
      $owners =  ammo_get_related_contacts($motion->id, 'motion', 'owner');
      // Get motion owners.
      if(empty($owners))
      {
        form_set_error('add_owner', t('Selecteer minstens één contact.'));
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
        $motion = entity_load_single('motion', $values['motion']);
        $form_state['storage']['motion'] = $values['motion'];
        $mapping = array(
          'meeting_id' => 'document',
          'branch_id' => 'branch_id',
          'backed' => 'backed',
          'consideration_opening' => 'consideration_opening',
          'consideration_body' => 'consideration_text',
          'follow_up_opening' => 'follow_up_opening',
          'follow_up_body' => 'follow_up_text',
          'supplement' => 'supplement',
          'advice' => 'advice',
          'advice_supplement' => 'advice_supplement',
          'state' => 'state',
        );
        foreach ($mapping as $database_field => $form_field) {
          $form_state['storage'][$form_field] = $motion->$database_field;
        }
        // Prefill advice fields.
        $prefill_fields = array(
          'consideration_opening' => 'consideration_opening',
          'consideration_body' => 'consideration_text',
          'follow_up_opening' => 'follow_up_opening',
          'follow_up_body' => 'follow_up_text',
        );
        foreach ($prefill_fields as $database_field => $form_field) {
          $advice_database_field = 'advice_' . $database_field;
          $advice_form_field = 'advice_' . $form_field;
          if (empty($motion->$advice_database_field)) {
            $form_state['storage'][$advice_form_field] = $motion->$database_field;
          }
          else {
            $form_state['storage'][$advice_form_field] = $motion->$advice_database_field;
          }
        }
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
