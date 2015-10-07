<?php

function ammo_amendment_form($form, &$form_state)
{
  $last_step = 7;
  // If $form_state['step'] is not set, we set it to 1
  $form_state['step'] = isset($form_state['step']) ? $form_state['step'] : 1;


  // Add a wrapper div that will be used by the Form API to update the form using AJAX
  $form['#prefix'] = '<div id="ajax_form_multistep_form">';
  $form['#suffix'] = '</div>';

  $form['title'] = array(
    '#type' => 'item',
    '#markup' => '<h2>Stap '.$form_state['step'].' / '.$last_step.'</h2>',
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
      '#title' => 'Meeting',
      '#options' => $options,
      '#empty_option' => 'kies:',
      '#default_value' => ammo_default($form_state, 'meeting'),
      '#required' => true,
      '#attributes' => array('onchange' => "jQuery('#next').mousedown()"),
    );
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
      '#attributes' => array('onchange' => "jQuery('#next').mousedown()"),
    );
    break;

  case 3:
    // Afdelingen filteren op gebruikers id.
    //$afdelingen = ammo_get_afdelingen(61619);
    $afdelingen = ammo_get_afdelingen();
    $form['branch_id'] = array(
      '#type' => 'select',
      '#title' => 'Afdeling',
      '#options' => $afdelingen,
      '#empty_option' => 'kies:',
      '#default_value' => ammo_default($form_state, 'branch_id'),
      '#required' => true,
      '#attributes' => array('onchange' => "jQuery('#next').mousedown()"),
    );
    break;

  case 4:
    $amendments = ammo_get_amendments($form_state['storage']['branch_id'], $form_state['storage']['document']);
    $form['amendment'] = array(
      '#type' => 'select',
      '#title' => 'Amendement nr.',
      '#options' => $amendments,
      '#empty_option' => 'nieuw',
      '#default_value' => ammo_default($form_state, 'amendment'),
      '#attributes' => array('onchange' => "jQuery('#next').mousedown()"),
    );
    break;

  case 5:
    $form['backed'] = array(
      '#type' => 'checkbox',
      '#title' => 'Amendement wordt ondersteund door afdeling',
      '#default_value' => ammo_default($form_state, 'backed'),
    );
    $form['owners'] = array(
      '#type' => 'fieldset',
    );
    $form['owners']['owner'] = array(
      '#type' => 'textfield',
      '#title' => 'naam / lidnummer',
      '#prefix' => '<p><strong>Ingediend door</strong></p>',
      '#autocomplete_path' => 'ammo/autocomplete_members/' . $form_state['storage']['branch_id'],
    );
    // create add participant button.
    $form['owners']['add_owner'] = array(
      '#type' => 'submit',
      '#value' => 'Voeg toe',
      '#name' => 'add_owner',
      '#validate' => array('ammo_amendment_form_validate'),
      '#submit' => array('ammo_amendment_form_submit'),
      '#ajax' => array
      (
        'wrapper' => 'ajax_form_multistep_form',
        'callback' => 'ammo_amendment_form_ajax_callback',
      ),
    );
    $owners = ammo_get_related_contacts($form_state['storage']['amendment'], 'amendment', 'owner');
    foreach ($owners as $owner) {
      $form['owners']['del_owner_' . $owner->contact_id] = array(
        '#type' => 'submit',
        '#value' => 'x',
        '#name' => 'del_owner_' . $owner->contact_id,
        '#prefix' => '<p>',
        '#suffix' =>  $owner->contact_id . '</p> ',
        '#ajax' => array(
          'callback' => 'ammo_amendment_form_ajax_callback',
          'wrapper' => 'ajax_form_multistep_form',
        ),
        '#attributes' => array('class' => array('ammo_owner_del_button')),
      );
    }
    break;

  case 6:
    $form['chapter'] = array(
      '#type' => 'textfield',
      '#title' => 'Hoofdstuk nr.',
      '#default_value' => ammo_default($form_state, 'chapter'),
    );
    $form['paragraph'] = array(
      '#type' => 'textfield',
      '#title' => 'Paragraaf nr.',
      '#default_value' => ammo_default($form_state, 'paragraph'),
    );
    $form['section'] = array(
      '#type' => 'textfield',
      '#title' => 'Sectie nr.',
      '#default_value' => ammo_default($form_state, 'section'),
    );
    $form['line'] = array(
      '#type' => 'textfield',
      '#title' => 'Regelnr.',
      '#default_value' => ammo_default($form_state, 'line'),
    );
    $form['action'] = array(
      '#type' => 'select',
      '#title' => 'Soort amendement',
      '#name' => 'action',
      '#options' => ammo_amendment_actions(),
      '#empty_option' => 'kies:',
      '#default_value' => ammo_default($form_state, 'action'),
    );
    $form['current_text'] = array(
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
    $form['new_text'] = array(
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

    $form['texts']['advice_current_text'] = array(
      '#type' => 'textarea',
      '#title' => 'Huidige tekst',
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
    $form['texts']['advice_new_text'] = array(
      '#type' => 'textarea',
      '#title' => 'Nieuwe tekst',
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
  // If we are on step 1, we do not want a back button
  if($form_state['step'] !== 1)
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
  // We only want a forward button if we are not on the last step of the form
  if($form_state['step'] !== $last_step)
  {
    // Hide next for first steps (javascript click onchange).
    $form['buttons']['forward'] = array
      (
        '#type' => 'submit',
        '#value' => t('Next'),
        '#name' => 'next',
        '#validate' => array('ammo_amendment_form_validate'),
        '#submit' => array('ammo_amendment_form_submit'),
        '#id' => 'next',
        '#ajax' => array
        (
          // We pass in the wrapper we created at the start of the form
          'wrapper' => 'ajax_form_multistep_form',
          // We pass a callback function we will use later to render the form for the user
          'callback' => 'ammo_amendment_form_ajax_callback',
        ),
      );
  }
  // We only want a submit button if we are on the last step of the form
  else
  {
    $form['buttons']['submit'] = array
      (
        '#type' => 'submit',
        '#value' => t('Submit'),
        '#name' => 'submit',
        '#ajax' => array
        (
          // We pass in the wrapper we created at the start of the form
          'wrapper' => 'ajax_form_multistep_form',
          // We pass a callback function we will use later to render the form for the user
          'callback' => 'ammo_amendment_form_ajax_callback',
        ),
      );
  }

  return $form;
}

function ammo_amendment_form_validate($form, &$form_state)
{
  // First we set up a switch for each of our three steps
  switch($form_state['step'])
  {
  case "1":
    // Check value
    if(FALSE)
    {
      form_set_error('step_1', t('Error message'));
    }

    break;
  case "2":
    // Check value
    if(FALSE)
    {
      form_set_error('step_2', t('Error message'));
    }

    break;
  case "3":
    // Check value
    if(FALSE)
    {
      form_set_error('step_3', t('Error message'));
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

  // First we determine which step we are on, and save the
  // submitted values to $form_state['storage']. This will
  // allow our submitted values to persist.
  $step = $form_state['step'];
  switch ($step) {
  case 1:
    $form_state['storage']['meeting'] = $values['meeting'];
    break;
  case 2:
    $form_state['storage']['document'] = $values['document'];
    break;
  case 3:
    $form_state['storage']['branch_id'] = $values['branch_id'];
    break;
  case 4:
    if (empty($values['amendment'])) {
      // Save new amendment.
      $mapping = array(
        'document_id' => 'document',
        'branch_id' => 'branch_id',
      );
      $new_amendment = ammo_store('amendment', NULL, $mapping, $form_state);
      $form_state['storage']['amendment'] = $new_amendment->id;
    }
    else {
      // Get and store values from existing amendment in form.
      $amendment = entity_load_single('amendment', $values['amendment']);
      $mapping = array(
        'document_id' => 'document',
        'branch_id' => 'branch_id',
        'backed' => 'backed',
        'chapter' => 'chapter',
        'paragraph' => 'paragraph',
        'section' => 'section',
        'line' => 'line',
        'action' => 'action',
        'current_text' => 'current_text',
        'new_text' => 'new_text',
        'supplement' => 'supplement',
        'advice' => 'advice',
        'advice_supplement' => 'advice_supplement',
        'advice_action' => 'advice_action',
        'advice_current_text' => 'advice_current_text',
        'advice_new_text' => 'advice_new_text',
        'state' => 'state',
      );
      foreach ($mapping as $database_field => $form_field) {
        $form_state['storage'][$form_field] = $amendment->$database_field;
      }
    }
    break;
  case 5:
    $mapping = array(
      'backed' => 'backed',
    );
    ammo_store('amendment', $storage['amendment'], $mapping, $form_state);
    break;
  case 6:
    $mapping = array(
      'chapter' => 'chapter',
      'paragraph' => 'paragraph',
      'section' => 'section',
      'line' => 'line',
      'action' => 'action',
      'current_text' => 'current_text',
      'new_text' => 'new_text',
      'supplement' => 'supplement',
    );
    ammo_store('amendment', $storage['amendment'], $mapping, $form_state);
    break;
  case 7:
    $mapping = array(
      'advice' => 'advice',
      'advice_supplement' => 'advice_supplement',
      'advice_action' => 'advice_action',
      'advice_current_text' => 'advice_current_text',
      'advice_new_text' => 'advice_new_text',
      'state' => 'state',
    );
    ammo_store('amendment', $storage['amendment'], $mapping, $form_state);
    // Reset form.
    $form_state['step'] = 1;
    $form_state['storage'] = array();
    break;
  }

  // Check triggers.
  if($trigger === 'add_owner')
  {
    dpm('Owner added!');
  }
  // Check to see if the next/forward button was clicked
  if($trigger === 'next')
  {
    // Increase the step by one, to move on to the next step
    $form_state['step']++;
  }

  $form_state['rebuild'] = TRUE;
}

function ammo_amendment_form_ajax_callback($form, &$form_state)
{
  return $form;
}
