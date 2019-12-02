<?php
function ammo_export_form($form, &$form_state) {

  $form['intro'] = array(
    '#markup' => '<p>Exporteer de amendementen en moties van een bijeenkomst als xml bestand geschikt voor Adobe InDesign.</p>',
  );

  $options = ammo_get_meetings();
  $form['meeting'] = array(
    '#type' => 'select',
    '#title' => 'Meeting',
    '#options' => $options,
    '#required' => true,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Exporteer',
  );
  return $form;
}

function ammo_export_form_validate($form, &$form_state) {
}

function ammo_export_form_submit($form, &$form_state) {
  header('Content-type: text/xml');
  header('Content-Disposition: attachement; filename="wijzigingsvoorstellen-moties.xml";');
  // open the "output" stream
  $f = fopen('php://output', 'w');

  $content = theme('export_meeting_xml', array('values' => $form_state['values']));

  fputs($f, $content);
  fclose($f);
  drupal_exit();
}
