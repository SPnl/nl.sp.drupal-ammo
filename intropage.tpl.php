<?php $enmoties = ($show_motions) ? ' en moties ': ''; ?>
<?php $ofmoties = ($show_motions) ? ' of moties ': ''; ?>
<div class="ammo-content">
  <h1>SP Wijzigingsvoorstellen<?php print $enmoties; ?></h1>
  <p><strong>Let op!</strong><br/>Wijzigingsvoorstellen<?php print $ofmoties; ?> van individuele leden kunnen uitsluitend worden ingediend via het landelijk secretariaat (secretariaat@sp.nl). Deze voorstellen dienen te worden ondersteund door ten minste 50 leden (handtekeningenlijst met naam en lidnummer bijvoegen).</p>
  <p><strong>U kunt in dit scherm:</strong></p>
  <ul>
    <li>Een overzicht van <?php print l('wijzigingsvoorstellen', 'ammo/amendments'); ?><? print ($show_motions) ? ' of ' . l('moties', 'ammo/motions') : ''; ?> bekijken.</li>
    <?php if ($add_access): ?>
      <li>Nieuwe <?php print l('wijzigingsvoorstellen', 'ammo/amendment'); ?><?php print ($show_motions) ? ' en ' . l('moties', 'ammo/motion') : ''; ?> indienen.</li>
    <?php endif; ?>
    <?php if ($superadmin_access): ?>
      <li>Een nieuwe <?php print l('bijeenkomst toevoegen', 'ammo/meeting'); ?>.</li>
      <li>Een nieuw <?php print l('stuk toevoegen', 'ammo/document'); ?> bij een bijeenkomst.</li>
    <?php endif; ?>
  </ul>
  <p><strong>In een overzicht kunt u:</strong></p>
  <ul>
    <?php if ($add_access): ?>
      <li>Wijzigingsvoorstellen<?php print $enmoties; ?> van andere afdelingen mede indienen.</li>
      <?php if (!$admin_access): ?>
        <li>Uw ingediende wijzigingsvoorstellen<?php print $enmoties; ?> aanpassen.</li>
      <?php endif; ?>
      <?php if ($admin_access): ?>
        <li>Ingediende wijzigingsvoorstellen<?php print  $enmoties; ?> aanpassen.</li>
        <li>Ingediende wijzigingsvoorstellen<?php print  $enmoties; ?> voorzien van advies.</li>
        <li>De status van ingediende wijzigingsvoorstellen<?php $enmoties; ?> aanpassen.</li>
      <?php endif; ?>
      <li>Uw ingediende wijzigingsvoorstellen<?php print $enmoties; ?> intrekken.</li>
    <?php endif; ?>
    <!--<li>Individueel ingediende wijzigingsvoorstellen en moties moties steunen.</li>-->
  </ul>
  <p><strong>Klik op een van de grijze tabjes bovenaan voor de gewenste functies.</strong></p>
</div>
