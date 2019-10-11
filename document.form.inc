<?php

function ammo_document_form($form, &$form_state) {
  if (empty($form_state['storage'])) {
    $form_state['storage'] = array();
  }
  $storage = $form_state['storage'];
  $form = array();

  $form['#prefix'] = '<div id="ammo-document-ajax">';
  $form['#suffix'] = '</div>';

  $form['form_title'] = array(
    '#type' => 'item',
    '#markup' => 'Voeg een document toe',
    '#prefix' => '<h2>',
    '#suffix' => '</h2>',
  );

  if (empty($storage['document_id'])) {
    $form['form_title']['#markup'] = 'Voeg een document toe';
  }
  else {
    $form['form_title']['#markup'] = 'Bewerk document';
  }

  if (empty($storage['meeting_id'])) {
    $meetings = entity_load('meeting');
    foreach ($meetings as $meeting) {
      $options[$meeting->id] = $meeting->title;
    }
    if (!empty($options)) {
      $form['meeting'] = array(
        '#type' => 'select',
        '#title' => 'Bijeenkomst',
        '#options' => $options,
        '#empty_option' => 'Kies',
      );

      $form['submit'] = array(
        '#type' => 'submit',
        '#value' => 'Verder',
        '#name' => 'submit_meeting',
        '#ajax' => array(
          'callback' => 'ammo_document_form_ajax',
          'wrapper' => 'ammo-document-ajax',
        ),
      );
    }
  }
  else {

    $form['title'] = array(
      '#type' => 'textfield',
      '#title' => 'Title',
    );

    $form['description'] = array(
      '#type' => 'textarea',
      '#title' => 'Description',
    );

    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => 'Indienen',
      '#name' => 'submit',
      '#ajax' => array(
        'callback' => 'ammo_document_form_ajax',
        'wrapper' => 'ammo-document-ajax',
      ),
    );

    if (empty($storage['document_id'])) {

      // New document.
      $documents = entity_load('document');
      $count = 0;
      foreach ($documents as $key => $document) {
        if ($document->meeting_id == $form_state['values']['meeting']) {
          $count ++;
        }
      }
      if ($count > 0) {
        $form['form_title2'] = array(
          '#type' => 'item',
          '#markup' => 'Bestaande documenten',
          '#prefix' => '<h2>',
          '#suffix' => '</h2>',
        );
      }
      foreach ($documents as $key => $document) {
        if ($document->meeting_id == $form_state['values']['meeting']) {
          $count ++;
          $form['documents'][$key]['del'] = array(
            '#type' => 'submit',
            '#value' => 'X',
            '#name' => 'del_' . $key,
            '#ajax' => array(
              'callback' => 'ammo_document_form_ajax',
              'wrapper' => 'ammo-document-ajax',
            ),
            '#prefix' => '<p>',
          );
          $form['documents'][$key]['edit'] = array(
            '#type' => 'submit',
            '#value' => 'Bewerk',
            '#name' => 'edit_' . $key,
            '#ajax' => array(
              'callback' => 'ammo_document_form_ajax',
              'wrapper' => 'ammo-document-ajax',
              'method' => 'replace',
              'effect' => 'fade',
              'event' => 'click',
            ),
            '#suffix' => $document->title.'</p>',
          );
        }
      }
    }
    else {
      // Editing existing document, set defaults.
      unset($form_state['input']);
      $form['title']['#default_value'] = $storage['document']->title;
      $form['description']['#default_value'] = $storage['document']->description;
    }
  }

  return $form;
}

function ammo_document_form_validate($form, &$form_state) {
  $storage = &$form_state['storage'];
  $values = $form_state['values'];
  $trigger = $form_state['triggering_element']['#name'];

  switch ($trigger) {
    case 'submit':
      if (empty($values['title'])) {
        form_set_error('title', 'Vul een titel in.');
      }
      if (empty($storage['document_id'])) {
        // New document.
        $unique_title = ammo_entity_check_title('document', $title);
        if ($unique_title === FALSE) {
          form_set_error('title', 'Titel is reeds in gebruik.');
        }
      }

    case 'submit_meeting':
      if (!empty($values['meeting'])) {
        $meeting = entity_load_single('meeting', $values['meeting']);
        $storage['meeting'] = $meeting;
        $storage['meeting_id'] = $meeting->id;
      }
      break;

  }
}

function ammo_document_form_submit($form, &$form_state) {
  $storage = &$form_state['storage'];
  $values = $form_state['values'];
  $trigger = $form_state['triggering_element']['#name'];

  // Check if delete document button was clicked.
  $pattern = '/^del_(\d+)$/';
  if (preg_match($pattern, $trigger, $matches)) {
    $result = entity_delete('document', $matches[1]);
    if (FALSE === $result) {
      drupal_set_message('Verwijderen document is mislukt.', 'error');
    }
    else {
      drupal_set_message('Document is verwijderd');
    }
    return $form;
  }

  // Check if edit document button was clicked.
  $pattern = '/^edit_(\d+)$/';
  if (preg_match($pattern, $trigger, $matches)) {
    $document = entity_load_single('document', $matches[1]);
    // Store document id.
    $storage['document_id'] = $document->id;
    $storage['document'] = $document;
  }

  switch ($trigger) {
    case 'submit':
      // Save document.
      $document_id = isset($storage['document_id']) ? $storage['document_id'] : NULL;
      $saved = ammo_save_entity('document', array(
        'meeting_id' => $storage['meeting_id'],
        'title' => $values['title'],
        'description' => $values['description'],
      ), $document_id);
      if ($saved) {
        $storage['document'] = $saved;
        $storage['document_id'] = $saved->id;
        drupal_set_message('Document is opgeslagen.');
        unset($form_state['input']);
        unset($form_state['storage']);
      }
      else {
        drupal_set_message('Document is niet opgeslagen.' , 'error');
      }
      break;
  }
  $form_state['rebuild'] = TRUE;
}

function ammo_document_form_ajax($form, &$form_state) {
  return $form;
}
