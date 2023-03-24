<?php

function ammo_meeting_form($form, &$form_state) {
  if (empty($form_state['storage'])) {
    $form_state['storage'] = array();
  }
  $storage = $form_state['storage'];
  $form = array();

  $form['#prefix'] = '<div id="ammo-meeting-ajax">';
  $form['#suffix'] = '</div>';

  if (empty($storage['meeting_id'])) {
    $form['meetings'] = array(
      '#title' => 'Bestaande bijeenkomsten',
      '#type' => 'fieldset',
    );

    // Editing existing meeting.
    $meetings = entity_load('meeting');
    foreach ($meetings as $key => $meeting) {
      $form['meetings'][$key]['del'] = array(
        '#type' => 'submit',
        '#value' => 'X',
        '#name' => 'del_' . $key,
        '#ajax' => array(
          'callback' => 'ammo_meeting_form_ajax',
          'wrapper' => 'ammo-meeting-ajax',
          'method' => 'replace',
          'effect' => 'fade',
          'event' => 'click',
        ),
        '#prefix' => '<p>',
      );
      $form['meetings'][$key]['edit'] = array(
        '#type' => 'submit',
        '#value' => 'Bewerk',
        '#name' => 'edit_' . $key,
        '#ajax' => array(
          'callback' => 'ammo_meeting_form_ajax',
          'wrapper' => 'ammo-meeting-ajax',
          'method' => 'replace',
          'effect' => 'fade',
          'event' => 'click',
        ),
        '#suffix' => $meeting->title.'</p>',
      );
    }
  }

  $form['form_title'] = array(
    '#type' => 'item',
    '#markup' => '<h2>Voeg een bijeenkomst toe</h2>',
  );

  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => 'Titel',
  );

  $form['description'] = array(
    '#type' => 'textarea',
    '#title' => 'Beschrijving',
  );

  $form['email'] = array(
    '#type' => 'textfield',
    '#title' => 'Contact e-mailadres',
  );

  $form['date'] = array(
    '#title' => 'Datum evenement',
    '#type' => 'date_popup',
    '#date_format' => 'd-m-Y',
  );

  $form['submission_closing_date'] = array(
    '#title' => 'Uiterste inlever/wijzigingsdatum',
    '#description' => 'Na deze datum kunnen er geen nieuwe wijzigingsvoorstellen en moties meer worden ingeleverd, en kunnen bestaande wijzigingsvoorstellen en moties niet meer worden gewijzigd.',
    '#type' => 'date_popup',
    '#date_format' => 'd-m-Y H:i',
  );

  $form['withdrawal_closing_date'] = array(
    '#title' => 'Uiterste intrek/ondersteuningsdatum',
    '#type' => 'date_popup',
    '#date_format' => 'd-m-Y H:i',
  );

  $form['hide_state'] = array(
    '#type' => 'checkbox',
    '#title' => 'Verberg status.',
  );

  $form['hide_advice'] = array(
    '#type' => 'checkbox',
    '#title' => 'Verberg advies.',
  );

  $form['hide_meeting'] = array(
    '#type' => 'checkbox',
    '#title' => 'Verberg bijeenkomst.',
  );

  $form['no_motions'] = array(
    '#type' => 'checkbox',
    '#title' => 'Geen moties.',
  );

  $form['admin_owner'] = array(
    '#type' => 'textfield',
    '#title' => 'Indienen amendementen, moties en wijzigingen door beheerders namens',
    '#default_value' => 'de congrescommissie',
    '#required' => TRUE,
  );
  if (!empty($form_state['values']['admin_owner'])) {
    $form['admin_owner']['#disabled'] = TRUE;
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Indienen',
    '#name' => 'submit',
    '#ajax' => array(
      'callback' => 'ammo_meeting_form_ajax',
      'wrapper' => 'ammo-meeting-ajax',
      'method' => 'replace',
      'effect' => 'fade',
      'event' => 'click',
    ),
  );

  // Set defaults.
  if (!empty($storage['meeting'])) {
    unset($form_state['input']);
    $meeting = $storage['meeting'];
    $form['form_title']['#markup'] = 'Bewerk bijeenkomst';
    $form['title']['#default_value'] = $meeting->title;
    $form['description']['#default_value'] = $meeting->description;
    $form['email']['#default_value'] = $meeting->email;
    $date = new DateTime($meeting->date);
    $form['date']['#default_value'] = $date->format('Y-m-d');;
    $date = new DateTime($meeting->withdrawal_closing_date);
    $form['withdrawal_closing_date']['#default_value'] = $date->format('Y-m-d H:i');
    $date = new DateTime($meeting->submission_closing_date);
    $form['submission_closing_date']['#default_value'] = $date->format('Y-m-d H:i');
    $form['hide_state']['#default_value'] = $meeting->hide_state;
    $form['hide_advice']['#default_value'] = $meeting->hide_advice;
    $form['hide_meeting']['#default_value'] = $meeting->hide_meeting;
    $form['no_motions']['#default_value'] = $meeting->no_motions;
    $form['admin_owner']['#default_value'] = $meeting->admin_owner;
    $form['admin_owner']['#disabled'] = TRUE;
  }
  return $form;
}

function ammo_meeting_form_validate($form, &$form_state) {
  $values = $form_state['values'];
  $storage = &$form_state['storage'];
  $trigger = $form_state['triggering_element']['#name'];

  $form_state['storage']['validated'] = 'YES';

  switch ($trigger) {
    // Save meeting.
    case 'submit':
      // Validate form values.
      $error = FALSE;
      if (empty($values['title'])) {
        $form['title']['#prefix'] = ammo_error('Vul een titel in.');
        $error = TRUE;
      }
      if (empty($values['date'])) {
        form_set_error('date', 'Kies een datum.');
      }
      if (empty($values['submission_closing_date'])) {
        form_set_error('submission_closing_date', 'Kies een laatste inleverdatum.');
        $error = TRUE;
      }
      if (empty($values['withdrawal_closing_date'])) {
        form_set_error('withdrawal_closing_date', 'Kies een laatste intrekdatum.');
        $error = TRUE;
      }
      if (empty($values['email']) || !valid_email_address($values['email'])) {
        form_set_error('email', 'Vul een geldig e-mailadres in.');
        $error = TRUE;
      }
      if (empty($storage['meeting_id'])) {
        // New meeting.
        $unique_title = ammo_entity_check_title('meeting', $values['title']);
        if ($unique_title === FALSE) {
          form_set_error('title', 'Titel is reeds in gebruik.');
          $error = TRUE;
        }
      }
      break;
  }
}

function ammo_meeting_form_submit($form, &$form_state) {
  $storage = &$form_state['storage'];
  $values = $form_state['values'];
  $trigger = $form_state['triggering_element']['#name'];

  // Check if delete meeting button was clicked.
  $pattern = '/^del_(\d+)$/';
  if (preg_match($pattern, $trigger, $matches)) {
    $result = entity_delete('meeting', $matches[1]);
    if (FALSE === $result) {
      drupal_set_message('Verwijderen bijeenkomst is mislukt.', 'error');
    }
    else {
      drupal_set_message('Bijeenkomst is verwijderd');
      $form_state['input'] = array();
    }
  }

  // Check if edit meeting button was clicked.
  $pattern = '/^edit_(\d+)$/';
  if (preg_match($pattern, $trigger, $matches)) {
    $meeting = entity_load_single('meeting', $matches[1]);
    $storage['meeting'] = $meeting;
    $storage['meeting_id'] = $meeting->id;
  }

  switch ($trigger) {
    // Save meeting.
    case 'submit':
      // Save meeting.
      $meeting_id = isset($storage['meeting_id']) ? $storage['meeting_id'] : NULL;
      $saved = ammo_save_entity('meeting', array(
        'title' => $values['title'],
        'description' => $values['description'],
        'date' => $values['date'],
        'submission_closing_date' => $values['submission_closing_date'],
        'withdrawal_closing_date' => $values['withdrawal_closing_date'],
        'email' => $values['email'],
        'no_member_owners' => 1,
        'hide_state' => $values['hide_state'],
        'hide_advice' => $values['hide_advice'],
        'hide_meeting' => $values['hide_meeting'],
        'no_motions' => $values['no_motions'],
        'admin_owner' => $values['admin_owner'],
      ), $meeting_id);
      if ($saved) {
        drupal_set_message('Bijeenkomst is opgeslagen.');
        unset($form_state['input']);
        unset($form_state['storage']);
      }
      else {
        drupal_set_message('Bijeenkomst is niet opgeslagen.', 'error');
      }
      break;
  }
  $form_state['rebuild'] = TRUE;
}

function ammo_meeting_form_ajax($form, &$form_state) {
  return $form;
}
