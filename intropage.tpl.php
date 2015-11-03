<div class="ammo-content">
  <h1>SP Amendementen en Moties</h1>
  <p><strong>U kunt in dit scherm:</strong></p>
  <ul>
    <li>Een overzicht van <?php print l('amendementen', 'ammo/amendments'); ?> of <?php print l('moties', 'ammo/motions'); ?> bekijken.</li>
    <?php if ($add_access): ?>
      <li>Nieuwe <?php print l('amendementen', 'ammo/amendment'); ?> en <?php print l('moties', 'ammo/motion'); ?> indienen.</li>
    <?php endif; ?>
    <?php if ($superadmin_access): ?>
      <li>Een nieuwe <?php print l('bijeenkomst toevoegen', 'ammo/meeting'); ?>.</li>
      <li>Een nieuw <?php print l('stuk toevoegen', 'ammo/document'); ?> bij een bijeenkomst.</li>
    <?php endif; ?>
  </ul>
  <p><strong>In een overzicht kunt u:</strong></p>
  <ul>
      <?php if ($add_access): ?>
      <li>Uw ingediende amendementen en moties intrekken.</li>
      <li>Amendementen en moties van andere afdelingen mede indienen.</li>
      <?php if (!$admin_access): ?>
        <li>Uw ingediende amendementen en moties aanpassen.</li>
      <?php endif; ?>
      <?php if ($admin_access): ?>
        <li>Ingediende amendementen en moties aanpassen.</li>
        <li>Ingediende amendementen en moties voorzien van advies.</li>
        <li>De status van ingediende amendementen en moties aanpassen.</li>
      <?php endif; ?>
    <?php endif; ?>
    <li>Individueel ingediende amendementen en moties moties steunen.</li>
  </ul>
  <p><strong>Klik op een van de grijze tabjes bovenaan voor de gewenste functies.</strong></p>
</div>
