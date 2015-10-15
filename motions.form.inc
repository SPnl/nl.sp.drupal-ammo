<?php

function ammo_overview_motions_form($form, &$form_state, $motion_id = NULL) {
  // Store destination before ajax call.
  ammo_set_destination($form_state);

  if (!empty($motion_id)) {
    $form['overview'] = array(
      '#type' => 'item',
      '#markup' => theme('overview_motions', array('meeting_id' => $motion_id, 'destination' => $form_state['storage']['destination'])),
    );
    return $form;
  }

  $form['#attached']['js'] = array(
    drupal_get_path('module', 'ammo') . '/ammo.js',
  );

  $form['#prefix'] = '<div id="ajax_form">';
  $form['#suffix'] = '</div>';
  ammo_select_meeting_form('motion', $form, $form_state);
  $form['submit'] = array
    (
      '#type' => 'submit',
      '#value' => 'kies',
      '#name' => 'action_button',
      '#id' => 'action_button',
      '#submit' => 'ammo_overview_motions_form_submit',
      '#ajax' => array
      (
        'wrapper' => 'ajax_form',
        'callback' => 'ammo_overview_motions_form_ajax_callback',
      ),
    );
  $form['overview'] = array(
    '#type' => 'item',
  );
  return $form;
}

function ammo_overview_motions_form_submit($form, &$form_state) {
  $form_state['rebuild'] = TRUE;
  return $form;
}

function ammo_overview_motions_form_ajax_callback($form, &$form_state) {
  $values = $form_state['values'];

  if (!empty($values['meeting'])) {
    $form_state['storage']['destination']['destination'] .= '/' . $values['meeting'];
  }
  $form['overview']['#markup'] = theme('overview_motions', array('meeting_id' => $values['meeting'], 'destination' => $form_state['storage']['destination']));
  return $form;
}
