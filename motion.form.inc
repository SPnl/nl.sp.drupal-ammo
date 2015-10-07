<?php

function ammo_motion_form($form, &$form_state) {
  $form = array();

  $form['#theme'] = 'motion_form';
  $form['#prefix'] = '<div id="ammo-motion-ajax">';
  $form['#suffix'] = '</div>';

  $form['form_title'] = array(
    '#type' => 'item',
    '#markup' => 'Voeg een motie toe',
    '#prefix' => '<h2>',
    '#suffix' => '</h2>',
  );

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
    '#ajax' => array(
      'callback' => 'ammo_motion_form_ajax',
      'wrapper' => 'ammo-motion-ajax',
      'method' => 'replace',
      'effect' => 'fade',
      'event' => 'change',
    ),
  );

  if (!empty($form_state['values']['meeting'])) {
    $form['branch_id'] = array(
      '#type' => 'textfield',
      '#title' => 'Afdelings id',
    );

    $form['backed'] = array(
      '#type' => 'checkbox',
      '#title' => 'ondersteund door afdeling',
    );

    $form['consideration_opening'] = array(
      '#type' => 'textfield',
      '#title' => 'Overweging openingszin',
    );

    $form['consideration_text'] = array(
      '#type' => 'textarea',
      '#title' => 'Overweging inhoud',
    );

    $form['follow_up_opening'] = array(
      '#type' => 'textfield',
      '#title' => 'Vervolg openingszin',
    );

    $form['follow_up_text'] = array(
      '#type' => 'textarea',
      '#title' => 'Vervolg inhoud',
    );

    $form['supplement'] = array(
      '#type' => 'textarea',
      '#title' => 'Toelichting',
    );

    $options = _motion_advice();
    $form['advice'] = array(
      '#type' => 'select',
      '#title' => 'Advies',
      '#options'=> $options,
      '#empty_option' => 'kies:',
    );

    $form['advice_supplement'] = array(
      '#type' => 'textarea',
      '#title' => 'Toelichting',
    );

    $form['advice_consideration_opening'] = array(
      '#type' => 'textfield',
      '#title' => 'Overweging openingszin',
    );

    $form['advice_consideration_text'] = array(
      '#type' => 'textarea',
      '#title' => 'Overweging inhoud',
    );

    $form['advice_follow_up_opening'] = array(
      '#type' => 'textfield',
      '#title' => 'Vervolg openingszin',
    );

    $form['advice_follow_up_text'] = array(
      '#type' => 'textarea',
      '#title' => 'Vervolg inhoud',
    );

    $options = _motion_states();
    $form['state'] = array(
      '#type' => 'select',
      '#title' => 'Status',
      '#options' => $options,
      '#empty_option' => 'kies:',
    );

    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => 'submit',
      '#name' => 'submit',
      '#ajax' => array(
        'callback' => 'ammo_motion_form_ajax',
        'wrapper' => 'ammo-motion-ajax',
        'method' => 'replace',
        'effect' => 'fade',
        'event' => 'click',
      ),
    );

    if (empty($form_state['motion_id'])) {

      // New motion.
      $motions = entity_load('motion');
      $count = 0;
      foreach ($motions as $key => $motion) {
        if ($motion->meeting_id == $form_state['values']['meeting']) {
          $count ++;
        }
      }
      if ($count > 0) {
        $form['form_title2'] = array(
          '#type' => 'item',
          '#markup' => 'Bestaande moties',
          '#prefix' => '<h2>',
          '#suffix' => '</h2>',
        );
      }
      foreach ($motions as $key => $motion) {
        if ($motion->meeting_id == $form_state['values']['meeting']) {
          $count ++;
          $form['motions'][$key]['del'] = array(
            '#type' => 'submit',
            '#value' => 'X',
            '#name' => 'del_' . $key,
            '#ajax' => array(
              'callback' => 'ammo_motion_form_ajax',
              'wrapper' => 'ammo-motion-ajax',
              'method' => 'replace',
              'effect' => 'fade',
              'event' => 'click',
            ),
            '#prefix' => '<p>',
          );
          $form['motions'][$key]['edit'] = array(
            '#type' => 'submit',
            '#value' => 'edit',
            '#name' => 'edit_' . $key,
            '#ajax' => array(
              'callback' => 'ammo_motion_form_ajax',
              'wrapper' => 'ammo-motion-ajax',
              'method' => 'replace',
              'effect' => 'fade',
              'event' => 'click',
            ),
            '#suffix' => $motion->title.'</p>',
          );
        }
      }
    }
  }

  return $form;
}

function ammo_motion_form_validate($form, &$form_state) {
}

function ammo_motion_form_ajax($form, &$form_state) {
  $values = $form_state['values'];
  $trigger = $form_state['triggering_element']['#name'];

  $meeting_id = $values['meeting'];

  // Check if delete motion button was clicked.
  $pattern = '/^del_(\d+)$/';
  if (preg_match($pattern, $trigger, $matches)) {
    $result = entity_delete('motion', $matches[1]);
    if (FALSE === $result) {
      drupal_set_message('Verwijderen motie is mislukt.', 'error');
    }
    else {
      drupal_set_message('Motie is verwijderd');
      $form = _motion_clear($form, $form_state);
    }
    return $form;
  }

  // Check if edit motion button was clicked.
  $pattern = '/^edit_(\d+)$/';
  if (preg_match($pattern, $trigger, $matches)) {
    $motion = entity_load_single('motion', $matches[1]);
    // Store motion id.
    $form_state['motion_id'] = $motion->id;
    $form = drupal_rebuild_form('ammo_motion_form', $form_state);
    $form['form_title']['#markup'] = 'Bewerk motie';
    $form['title']['#value'] = $motion->title;
    $form['description']['#value'] = $motion->description;
    return $form;
  }

  switch ($trigger) {
    // Save motion.
  case 'submit':
    // Validate form values.
    $error = FALSE;
    if (empty($values['title'])) {
      $form['title']['#prefix'] = ammo_error('Vul een titel in.');
      $error = TRUE;
    }
    if (empty($form_state['motion_id'])) {
      // New motion.
      $unique_title = ammo_entity_check_title('motion', $title);
      if ($unique_title === FALSE) {
        $form['title']['#prefix'] = ammo_error('Titel is reeds in gebruik.');
        $error = TRUE;
      }
    }
    if ($error) {
      return $form;
    }

    // Save motion.
    $motion_id = isset($form_state['motion_id']) ? $form_state['motion_id'] : NULL;
    $saved = ammo_save_entity('motion', array(
      'meeting_id' => $meeting_id,
      'title' => $title,
      'description' => $description,
    ), $motion_id);    
    if ($saved) {
      drupal_set_message('Motie is opgeslagen.');
      $form = _motion_clear($form, $form_state);
    }
    else {
      drupal_set_message('Motie is niet opgeslagen.' , 'error');
    }
    break;
  }
  return $form;
}

function _motion_clear($form, &$form_state) {
  unset($form_state['input']['title']);
  unset($form_state['input']['description']);
  unset($form_state['values']['title']);
  unset($form_state['values']['description']);
  unset($form_state['motion_id']);
  return drupal_rebuild_form('ammo_motion_form', $form_state);
}
