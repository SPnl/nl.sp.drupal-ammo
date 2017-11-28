<div class="ammo-content">
  <h1>SP Wijzigingsvoorstellen en Moties</h1>
  <p><strong>Let op!</strong><br/>Wijzigingsvoorstellen of moties van individuele leden kunnen uitsluiten worden ingediend via het landelijk secretariaat (secretariaat@sp.nl). Deze voorstellen dienen te worden ondersteund door ten minste 50 leden (handtekeningenlijst met naam en lidnummer bijvoegen).</p>
  <p><strong>U kunt in dit scherm:</strong></p>
  <ul>
    <li>Een overzicht van <?php print l('wijzigingsvoorstellen', 'ammo/amendments'); ?> of <?php print l('moties', 'ammo/motions'); ?> bekijken.</li>
    <?php if ($add_access): ?>
      <li>Nieuwe <?php print l('wijzigingsvoorstellen', 'ammo/amendment'); ?> en <?php print l('moties', 'ammo/motion'); ?> indienen.</li>
    <?php endif; ?>
    <?php if ($superadmin_access): ?>
      <li>Een nieuwe <?php print l('bijeenkomst toevoegen', 'ammo/meeting'); ?>.</li>
      <li>Een nieuw <?php print l('stuk toevoegen', 'ammo/document'); ?> bij een bijeenkomst.</li>
    <?php endif; ?>
  </ul>
  <p><strong>In een overzicht kunt u:</strong></p>
  <ul>
    <?php if ($add_access): ?>
      <li>Wijzigingsvoorstellen en moties van andere afdelingen mede indienen.</li>
      <?php if (!$admin_access): ?>
        <li>Uw ingediende wijzigingsvoorstellen en moties aanpassen.</li>
      <?php endif; ?>
      <?php if ($admin_access): ?>
        <li>Ingediende wijzigingsvoorstellen en moties aanpassen.</li>
        <li>Ingediende wijzigingsvoorstellen en moties voorzien van advies.</li>
        <li>De status van ingediende wijzigingsvoorstellen en moties aanpassen.</li>
      <?php endif; ?>
      <li>Uw ingediende wijzigingsvoorstellen en moties intrekken.</li>
    <?php endif; ?>
    <!--<li>Individueel ingediende wijzigingsvoorstellen en moties moties steunen.</li>-->
  </ul>
  <p><strong>Klik op een van de grijze tabjes bovenaan voor de gewenste functies.</strong></p>
</div>
