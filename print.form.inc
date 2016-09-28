<?php
function ammo_print_form($form, &$form_state) {
  // Add a wrapper div that will be used by the Form API to update the form using AJAX
  $form['#prefix'] = '<div id="ajax-print-form-wrapper">';
  $form['#suffix'] = '</div>';

  ammo_select_meeting_form($form, $form_state);
  $form['meeting']['#ajax'] = array(
    'wrapper' => 'ajax-print-form-wrapper',
    'callback' => 'ammo_print_form_ajax_callback',
  );

  if (!empty($form_state['values']['meeting'])) {
    $form['type'] = array(
      '#type' => 'radios',
      '#title' => 'Wat wilt u printen?',
      '#name' => 'type',
      '#required' => TRUE,
      '#options' => array(
        'amendments' => 'amendementen',
        'motions' => 'moties',
      ),
      '#ajax' => array(
        'wrapper' => 'ajax-print-form-wrapper',
        'callback' => 'ammo_print_form_ajax_callback',
      ),
    );
  }

  switch ($form_state['values']['type']) {
  case 'amendments':
    $form['amendments_order'] = array(
      '#type' => 'radios',
      '#title' => 'Sorteer de amendementen op volgorde van',
      '#required' => TRUE,
      '#options' => array(
        'location' => 'de plaats in het document',
        'number' => 'het amendement nummer',
      ),
      '#default_value' => 'location',
      '#ajax' => array(
        'wrapper' => 'ajax-print-form-wrapper',
        'callback' => 'ammo_print_form_ajax_callback',
      ),
    );
    unset($form_state['values']['motions_order']);
    break;
  case 'motions':
    $form['motions_order'] = array(
      '#type' => 'radios',
      '#title' => 'Sorteer de moties op volgorde van',
      '#required' => TRUE,
      '#options' => array(
        'branch' => 'de afdeling',
        'number' => 'het nummer van de motie',
      ),
      '#default_value' => 'branch',
      '#ajax' => array(
        'wrapper' => 'ajax-print-form-wrapper',
        'callback' => 'ammo_print_form_ajax_callback',
      ),
    );
    unset($form_state['values']['amendments_order']);
    break;
  }

  if (!empty($form_state['values']['type'])) {
    $form['writerordoc'] = array(
      '#type' => 'radios',
      '#title' => 'Bestand voor',
      '#name' => 'writerordoc',
      '#required' => TRUE,
      '#options' => array(
        'writer' => 'Libreoffice writer',
        'doc' => 'Mircosoft word',
      ),
      '#default_value' => 'writer',
      '#ajax' => array(
        'wrapper' => 'ajax-print-form-wrapper',
        'callback' => 'ammo_print_form_ajax_callback',
      ),
    );
  }

  if (!empty($form_state['values']['type'])) {
    $form['notespace'] = array(
      '#type' => 'checkbox',
      '#title' => 'Ruimte voor notities toevoegen',
    );
  }

  if (!empty($form_state['values']['type'])) {
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => 'Print',
      '#name' => 'action_button',
      '#id' => 'action_button',
      '#validate' => array('ammo_print_form_validate'),
      '#submit' => array('ammo_print_form_submit'),
    );
  }

  return $form;
}

function ammo_print_form_validate($form, &$form_state) {
}

function ammo_print_form_submit($form, &$form_state) {
  header('Content-type: application/vnd.ms-word');
  header('Content-Disposition: attachement; filename="ammo.doc";');
  // open the "output" stream
  $f = fopen('php://output', 'w');

  switch ($form_state['values']['type']) {
  case 'amendments':
    switch ($form_state['values']['amendments_order']) {
    case 'location':
      $content = theme('print_amendments_by_location', array('values' => $form_state['values']));
      break;
    case 'number':
      $content = theme('print_amendments_by_number', array('values' => $form_state['values']));
      break;
    }
    break;
  case 'motions':
    switch ($form_state['values']['motions_order']) {
    case 'branch':
      $content = theme('print_motions_by_branch', array('values' => $form_state['values']));
      break;
    case 'number':
      $content = theme('print_motions_by_number', array('values' => $form_state['values']));
      break;
    }
    break;
  }

  if ($form_state['values']['writerordoc'] == 'doc') {
    $content = ammo_encodeToIso($content);
  }

  fputs($f, $content);
  fclose($f);
  drupal_exit();
}

function ammo_print_form_ajax_callback($form, &$form_state) {
  return $form;
}

function ammo_encodeToIso($string) {
  return mb_convert_encoding($string, "ISO-8859-1", mb_detect_encoding($string, "UTF-8, ISO-8859-1, ISO-8859-15", true));
}
