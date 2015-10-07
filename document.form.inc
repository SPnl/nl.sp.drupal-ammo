<?php

function ammo_document_form($form, &$form_state) {
  $form = array();

  $form['#theme'] = 'document_form';
  $form['#prefix'] = '<div id="ammo-document-ajax">';
  $form['#suffix'] = '</div>';

  $form['form_title'] = array(
    '#type' => 'item',
    '#markup' => 'Voeg een document toe',
    '#prefix' => '<h2>',
    '#suffix' => '</h2>',
  );

  $meetings = entity_load('meeting');
  foreach ($meetings as $meeting) {
    $options[$meeting->id] = $meeting->title;
  }
  $form['meeting'] = array(
    '#type' => 'select',
    '#title' => 'Meeting',
    '#options' => $options,
    '#empty_option' => 'kies:',
    '#ajax' => array(
      'callback' => 'ammo_document_form_ajax',
      'wrapper' => 'ammo-document-ajax',
      'method' => 'replace',
      'effect' => 'fade',
      'event' => 'change',
    ),
  );

  if (!empty($form_state['values']['meeting'])) {
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
      '#value' => 'submit',
      '#name' => 'submit',
      '#ajax' => array(
        'callback' => 'ammo_document_form_ajax',
        'wrapper' => 'ammo-document-ajax',
        'method' => 'replace',
        'effect' => 'fade',
        'event' => 'click',
      ),
    );

    if (empty($form_state['document_id'])) {

      // Editing existing document.
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
              'method' => 'replace',
              'effect' => 'fade',
              'event' => 'click',
            ),
            '#prefix' => '<p>',
          );
          $form['documents'][$key]['edit'] = array(
            '#type' => 'submit',
            '#value' => 'edit',
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
  }

  return $form;
}

function ammo_document_form_validate($form, &$form_state) {
}

function ammo_document_form_ajax($form, &$form_state) {
  $values = $form_state['values'];
  $trigger = $form_state['triggering_element']['#name'];

  $meeting_id = $values['meeting'];
  $document = $values['document'];
  $title = $values['title'];
  $description = $values['description'];

  // Check if delete document button was clicked.
  $pattern = '/^del_(\d+)$/';
  if (preg_match($pattern, $trigger, $matches)) {
    $result = entity_delete('document', $matches[1]);
    if (FALSE === $result) {
      drupal_set_message('Verwijderen document is mislukt.', 'error');
    }
    else {
      drupal_set_message('Document is verwijderd');
      $form = _document_clear($form, $form_state);
    }
    return $form;
  }

  // Check if edit document button was clicked.
  $pattern = '/^edit_(\d+)$/';
  if (preg_match($pattern, $trigger, $matches)) {
    $document = entity_load_single('document', $matches[1]);
    // Store document id.
    $form_state['document_id'] = $document->id;
    $form = drupal_rebuild_form('ammo_document_form', $form_state);
    $form['form_title']['#markup'] = 'Bewerk document';
    $form['title']['#value'] = $document->title;
    $form['description']['#value'] = $document->description;
    return $form;
  }

  switch ($trigger) {
  // Save document.
  case 'submit':
    // Validate form values.
    $error = FALSE;
    if (empty($values['title'])) {
      $form['title']['#prefix'] = ammo_error('Vul een titel in.');
      $error = TRUE;
    }
    if (empty($form_state['document_id'])) {
      // New document.
      $unique_title = ammo_entity_check_title('document', $title);
      if ($unique_title === FALSE) {
        $form['title']['#prefix'] = ammo_error('Titel is reeds in gebruik.');
        $error = TRUE;
      }
    }
    if ($error) {
      return $form;
    }

    // Save document.
    $document_id = isset($form_state['document_id']) ? $form_state['document_id'] : NULL;
    $saved = ammo_save_entity('document', array(
        'meeting_id' => $meeting_id,
        'title' => $title,
        'description' => $description,
      ), $document_id);    
    if ($saved) {
      drupal_set_message('Document is opgeslagen.');
      $form = _document_clear($form, $form_state);
    }
    else {
      drupal_set_message('Document is niet opgeslagen.' , 'error');
    }
    break;
  }
  return $form;
}

function _document_clear($form, &$form_state) {
  unset($form_state['input']['title']);
  unset($form_state['input']['description']);
  unset($form_state['values']['title']);
  unset($form_state['values']['description']);
  unset($form_state['document_id']);
  return drupal_rebuild_form('ammo_document_form', $form_state);
}
