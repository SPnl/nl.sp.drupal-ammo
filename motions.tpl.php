<?php if (!empty($meetings)) : ?>
  <h1 id="inhoud">Inhoud</h1>
  <?php if (!empty($meetings[1]['branches'])) : ?>

    <ul>
      <?php foreach ($meetings[1]['branches'] as $branch) : ?>
        <li>
          <a href="#branch<?php print $branch['id']; ?>"><?php print $branch['branch_display_name']; ?></a>
          <ul>
            <?php foreach ($branch['motions'] as $motion) : ?>
              <li>
                <a href="#motion<?php print $motion['id']; ?>">Motie nr. <?php print $motion['id']; ?></a>
              </li>
            <?php endforeach; ?>
          </ul>
        </li>
      <?php endforeach; ?>
    </ul>

    <ul>
      <?php foreach ($meetings[1]['branches'] as $branch) : ?>
        <li>
          <h2 id="branch<?php print $branch['id']; ?>"><?php print $branch['branch_display_name']; ?></h2>
          <ul>
            <?php foreach ($branch['motions'] as $motion) : ?>
              <li>
                <?php print theme('motion', array('entity_id' => $motion['id'], 'destination' => $destination)); ?>
              </li>
            <?php endforeach; ?>
          </ul>
        </li>
      <?php endforeach; ?>
    </ul>

  <?php endif; ?>
<?php endif; ?>



