<?php

function ammo_meeting_form($form, &$form_state) {
  $form = array();

  $form['#theme'] = 'meeting_form';
  $form['#prefix'] = '<div id="ammo-meeting-ajax">';
  $form['#suffix'] = '</div>';

  $form['form_title'] = array(
    '#type' => 'item',
    '#markup' => 'Voeg een bijeenkomst toe',
    '#prefix' => '<h2>',
    '#suffix' => '</h2>',
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

  $form['no_member_owners'] = array(
    '#type' => 'checkbox',
    '#title' => 'Geen individuele indieners.',
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
    '#value' => 'submit',
    '#name' => 'submit',
    '#ajax' => array(
      'callback' => 'ammo_meeting_form_ajax',
      'wrapper' => 'ammo-meeting-ajax',
      'method' => 'replace',
      'effect' => 'fade',
      'event' => 'click',
    ),
  );

  if (empty($form_state['meeting_id'])) {
    $form['form_title2'] = array(
      '#type' => 'item',
      '#markup' => 'Bestaande bijeenkomsten',
      '#prefix' => '<h2>',
      '#suffix' => '</h2>',
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
        '#value' => 'edit',
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

  return $form;
}

function ammo_meeting_form_validate($form, &$form_state) {
}

function ammo_meeting_form_ajax($form, &$form_state) {
  $values = $form_state['values'];
  $trigger = $form_state['triggering_element']['#name'];

  $title = $values['title'];
  $description = $values['description'];
  $date = $values['date'];
  $withdrawal_closing_date = $values['withdrawal_closing_date'];
  $submission_closing_date = $values['submission_closing_date'];
  $no_member_owners = $values['no_member_owners'];
  $hide_state = $values['hide_state'];
  $hide_advice = $values['hide_advice'];
  $hide_meeting = $values['hide_meeting'];
  $no_motions = $values['no_motions'];
  $email = $values['email'];
  $admin_owner = $values['admin_owner'];

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
      $form = drupal_rebuild_form('ammo_meeting_form', $form_state);
    }
    return $form;
  }

  // Check if edit meeting button was clicked.
  $pattern = '/^edit_(\d+)$/';
  if (preg_match($pattern, $trigger, $matches)) {
    $meeting = entity_load_single('meeting', $matches[1]);
    // Store meeting id.
    $form_state['meeting_id'] = $meeting->id;
    $form = drupal_rebuild_form('ammo_meeting_form', $form_state);
    $form['form_title']['#markup'] = 'Bewerk bijeenkomst';
    $form['title']['#value'] = $meeting->title;
    $form['description']['#value'] = $meeting->description;
    $form['email']['#value'] = $meeting->email;
    $date = new DateTime($meeting->date);
    $form['date']['date']['#value'] = $date->format('d-m-Y');;
    $date = new DateTime($meeting->withdrawal_closing_date);
    $form['withdrawal_closing_date']['date']['#value'] = $date->format('d-m-Y');
    $form['withdrawal_closing_date']['time']['#value'] = $date->format('H:i');
    $date = new DateTime($meeting->submission_closing_date);
    $form['submission_closing_date']['date']['#value'] = $date->format('d-m-Y');
    $form['submission_closing_date']['time']['#value'] = $date->format('H:i');
    $form['no_member_owners']['#checked'] = $meeting->no_member_owners;
    $form['hide_state']['#checked'] = $meeting->hide_state;
    $form['hide_advice']['#checked'] = $meeting->hide_advice;
    $form['hide_meeting']['#checked'] = $meeting->hide_meeting;
    $form['no_motions']['#checked'] = $meeting->no_motions;
    $form['admin_owner']['#value'] = $meeting->admin_owner;
    $form['admin_owner']['#disabled'] = TRUE;
    return $form;
  }

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
      $form['date']['#prefix'] = ammo_error('Kies een datum.');
      $error = TRUE;
    }
    if (empty($values['submission_closing_date'])) {
      $form['date']['#prefix'] = ammo_error('Kies een laatste inleverdatum.');
      $error = TRUE;
    }
    if (empty($values['withdrawal_closing_date'])) {
      $form['date']['#prefix'] = ammo_error('Kies een laatste intrekdatum.');
      $error = TRUE;
    }
    if (empty($values['email']) || !valid_email_address($values['email'])) {
      $form['email']['#prefix'] = ammo_error('Vul een geldig e-mailadres in.');
      $error = TRUE;
    }
    if (empty($form_state['meeting_id'])) {
      // New meeting.
      $unique_title = ammo_entity_check_title('meeting', $title);
      if ($unique_title === FALSE) {
        $form['title']['#prefix'] = ammo_error('Titel is reeds in gebruik.');
        $error = TRUE;
      }
    }
    if ($error) {
      return $form;
    }

    // Save meeting.
    $meeting_id = isset($form_state['meeting_id']) ? $form_state['meeting_id'] : NULL;
    $saved = ammo_save_entity('meeting', array(
        'title' => $title,
        'description' => $description,
        'date' => $date,
        'submission_closing_date' => $submission_closing_date,
        'withdrawal_closing_date' => $withdrawal_closing_date,
        'email' => $email,
        'no_member_owners' => $no_member_owners,
        'hide_state' => $hide_state,
        'hide_advice' => $hide_advice,
        'hide_meeting' => $hide_meeting,
        'no_motions' => $no_motions,
        'admin_owner' => $admin_owner,
      ), $meeting_id);
    if ($saved) {
      drupal_set_message('Bijeenkomst is opgeslagen.');
      $form_state['input'] = array();
      unset($form_state['meeting_id']);
      $form = drupal_rebuild_form('ammo_meeting_form', $form_state);
    }
    else {
      drupal_set_message('Bijeenkomst is niet opgeslagen.', 'error');
    }
    break;
  }
  return $form;
}
