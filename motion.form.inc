<?php

function ammo_motion_form($form, &$form_state, $form_action, $path_id = NULL)
{
  global $user;

  // Store destination before ajax call.
  ammo_set_destination($form_state);

  // Check if motion is set in path and preset form if so.
  if (!empty($path_id) && !empty($form_action) && empty($form_state['storage']['first_step'])) {
    switch ($form_action) {
    case 'edit':
      if (ammo_preset_form('motion', $path_id, $form_state)) {
        // Check access right.
        if (
          isset($form_state['storage']['meeting']) &&
          isset($form_state['storage']['branch_id']) &&
          ammo_edit_access($form_state['storage']['meeting'], $form_state['storage']['branch_id'])
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
        if (ammo_preset_form('motion', $path_id, $form_state)) {
          if (ammo_support_access($form_state['storage']['meeting']) || ammo_superadmin_access()) {
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
      $form = ammo_view_entity_form('motion', $path_id);
      return $form;
      break;
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
    '#markup' => '<h2>Stap '.($form_state['step'] - $form_state['storage']['first_step'] + 1).' / '.($form_state['storage']['last_step'] - $form_state['storage']['first_step'] + 1).'</h2>',
  );

  // Depending on which step of the form we are on, we output different form fields
  switch($form_state['step'])
  {

  case 1:
    ammo_select_meeting_form($form, $form_state);
    break;

  case 2:
    ammo_checked_form($form, $form_state);
    break;

  case 3:
    ammo_select_afdeling_form($form, $form_state);
    break;

  case 4:
    ammo_select_ammo_form('motion', $form, $form_state, $form_state['storage']['meeting']);
    break;

  case 5:
    ammo_select_owner_form('motion', $form, $form_state);
    break;

  case 6:
    $ammo_default = ammo_default($form_state, 'consideration_opening');
    $default = (empty($ammo_default)) ? 'Overwegende dat' : $ammo_default;
    $form['consideration_opening'] = array(
      '#type' => 'textfield',
      '#title' => '',
      '#default_value' => $default,
      '#size' => 50,
      '#maxlength' => 80,
    );

    $form['consideration_text'] = array(
      '#type' => 'textarea',
      '#title' => '',
      '#default_value' => ammo_default($form_state, 'consideration_text'),
    );

    $ammo_default = ammo_default($form_state, 'follow_up_opening');
    $default = (empty($ammo_default)) ? 'Verzoekt ...' : $ammo_default;
    $form['follow_up_opening'] = array(
      '#type' => 'textfield',
      '#title' => '',
      '#default_value' => $default,
      '#size' => 50,
      '#maxlength' => 80,
    );

    $form['follow_up_text'] = array(
      '#type' => 'textarea',
      '#title' => '',
      '#default_value' => ammo_default($form_state, 'follow_up_text'),
    );

    $form['supplement'] = array(
      '#type' => 'textarea',
      '#title' => 'Toelichting',
      '#default_value' => ammo_default($form_state, 'supplement'),
    );
    break;

  case 7:
    $form['item'] = array(
      '#type' => 'item',
      '#markup' => theme('motion', array('entity_id' => $form_state['storage']['motion'], 'no_links' => TRUE)),
    );

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
    $form['texts']['original_consideration_text'] = array(
      '#type' => 'item',
      '#markup' => '<p><strong>Overweging inhoud orgineel</strong><br/>' . check_plain($form_state['storage']['consideration_text']),
    );
    $form['texts']['original_follow_up_opening'] = array(
      '#type' => 'item',
      '#markup' => '<p><strong>Vervolg openingszin orgineel</strong><br/>' . check_plain($form_state['storage']['follow_up_opening']),
    );
    $form['texts']['original_follow_up_text'] = array(
      '#type' => 'item',
      '#markup' => '<p><strong>Vervolg inhoud orgineel</strong><br/>' . check_plain($form_state['storage']['follow_up_text']),
    );

    $form['texts']['advice_consideration_opening'] = array(
      '#type' => 'textfield',
      '#title' => 'Overweging openingszin in advies',
      '#default_value' => ammo_default($form_state, 'advice_consideration_opening'),
      '#size' => 50,
      '#maxlength' => 80,
    );
    $form['texts']['advice_consideration_text'] = array(
      '#type' => 'textarea',
      '#title' => 'Overweging inhoud in advies',
      '#default_value' => ammo_default($form_state, 'advice_consideration_text'),
    );
    $form['texts']['advice_follow_up_opening'] = array(
      '#type' => 'textfield',
      '#title' => 'Vervolg openingszin in advies',
      '#default_value' => ammo_default($form_state, 'advice_follow_up_opening'),
      '#size' => 50,
      '#maxlength' => 80,
    );
    $form['texts']['advice_follow_up_text'] = array(
      '#type' => 'textarea',
      '#title' => 'Vervolg inhoud in advies',
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
      '#options' => ammo_states(),
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
  if(
    $form_state['step'] > $form_state['storage']['first_step'] &&
    $form_state['step'] <> 5
  )
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
  switch($step) {
  case 5:
    switch ($trigger) {
    case 'action_button':
      if (!$backed) {
        $motion = entity_load_single('motion', $storage['motion']);
        $owners =  ammo_get_related_contacts($motion->id, 'motion', 'owner', 'member');
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
    break;
  case 6:
    if (empty($values['consideration_opening'])) {
      form_set_error('consideration_opening', 'Vul de overweging openingszin in.');
    }
    if (empty($values['consideration_text'])) {
      form_set_error('consideration_text', 'Vul de overweging inhoud in.');
    }
    if (empty($values['follow_up_opening'])) {
      form_set_error('follow_up_opening', 'Vul de vervolg openingszin in.');
    }
    if (empty($values['follow_up_text'])) {
      form_set_error('follow_up_text', 'Vul de vervolg inhoud in.');
    }
    break;
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
    case 3:
      $form_state['storage']['branch_id'] = $values['branch_id'];
      $form_state['storage']['branch_display_name'] = $form['branch_id']['#options'][$values['branch_id']];
      break;
    case 4:
      if (empty($values['motion'])) {
        // Save new motion.
        $form_state['storage']['state'] = 'concept';
        $mapping = array(
          'meeting_id' => 'meeting',
          'state' => 'state',
        );
        $new_motion = ammo_store('motion', NULL, $mapping, $form, $form_state);
        $form_state['storage']['motion'] = $new_motion->id;
        $form_state['storage']['new'] = TRUE;
      }
      else {
        // Get and store values from existing motion in form.
        ammo_populate_form('motion', $values['motion'], $form_state);
      }
      break;
    case 5:
      if ($values['backed']) {
        // Check if already set owner.
        if (!ammo_is_related($storage['motion'], 'motion', 'owner', 'branch', $storage['branch_id'])) {
          // Set owner.
          $entity = ammo_save_relation('owner', 'motion', $storage['motion'], $storage['branch_id'], 'branch', $storage['branch_display_name'], $storage['branch_id'], $storage['branch_display_name']);
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
        'consideration_opening' => 'consideration_opening',
        'consideration_body' => 'consideration_text',
        'follow_up_opening' => 'follow_up_opening',
        'follow_up_body' => 'follow_up_text',
        'supplement' => 'supplement',
        'state' => 'state',
      );
      ammo_store('motion', $storage['motion'], $mapping, $form, $form_state);
      break;
    case 7:
      $mapping = array(
        'advice' => 'advice',
        'advice_supplement' => 'advice_supplement',
        'advice_consideration_opening' => 'advice_consideration_opening',
        'advice_consideration_body' => 'advice_consideration_text',
        'advice_follow_up_opening' => 'advice_follow_up_opening',
        'advice_follow_up_body' => 'advice_follow_up_text',
        'state' => 'state',
      );
      ammo_store('motion', $storage['motion'], $mapping, $form, $form_state);
      break;
    }
    // Set next page, or finish form.
    if ($storage['last_step'] == $step) {
      // Redirect user if destination is set.
      ammo_ajax_redirect($form_state['storage']['destination']['destination']);
      // Otherwise reset form.
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
    case 5:
      $form_state['storage']['new'] = FALSE;
      $motion = entity_load_single('motion', $storage['motion']);
      $display_name = ammo_contact_get_display_name($values['owner']);
      // Sla nieuwe owner op.
      $owner = ammo_save_relation('owner', 'motion', $motion->id, $values['owner'], 'member', $display_name, $storage['branch_id'], $storage['branch_display_name']);
      if ($owner) {
        drupal_set_message('Contact is toegevoegd.');
        // Verwijder afdelingseigenaar.
        $contact_relation_id = ammo_is_related($storage['motion'], 'motion', 'owner', 'branch', $storage['branch_id']); 
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

function ammo_motion_form_ajax_callback($form, &$form_state)
{
  return $form;
}
