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
    '#title' => 'Title',
  );

  $form['description'] = array(
    '#type' => 'textarea',
    '#title' => 'Description',
  );

  $form['date'] = array(
    '#type' => 'date_popup',
    '#date_format' => 'd-m-Y',
  );

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
    $date = new DateTime($meeting->date);
    $form['date']['date']['#value'] = $date->format('d-m-Y');;
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
      ), $meeting_id);    
    if ($saved) {
      drupal_set_message('Bijeenkomst is opgeslagen.');
      $form_state['input'] = array();
      unset($form_state['meeting_id']);
      $form = drupal_rebuild_form('ammo_meeting_form', $form_state);
    }
    else {
      drupal_set_message('Bijeenkomst is niet opgeslagen.' , 'error');
    }
    break;
  }
  return $form;
}
