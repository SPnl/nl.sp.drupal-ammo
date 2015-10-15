<h1 id="inhoud">Inhoud</h1>
<?php if (!empty($meetings)) : ?>
  <?php if (!empty($meetings[1]['documents'])) : ?>

    <ul class="first">
      <?php foreach ($meetings[1]['documents'] as $document) : ?>
        <li>
          <a href="#document<?php print $document['id']; ?>"><?php print $document['title']; ?></a>
          <ul>
            <a href="#algemeen">Algemeen</a>
            <ul><ul>
              <?php foreach ($document['type']['general']['amendments'] as $amendment) : ?>
                <li>
                  <a href="#amendment<?php print $amendment['id']; ?>">Amendement nr. <?php print $amendment['id']; ?></a>
                </li>
              <?php endforeach; ?>
            </ul></ul>
            <?php foreach ($document['type']['specific']['chapters'] as $chapter) : ?>
              <li>
                <a href="#chapter<?php print $chapter['nr']; ?>">Hoofdstuk <?php print $chapter['nr']; ?></a>
                <ul>
                  <?php foreach ($chapter['pages'] as $page) : ?>
                    <li>
                      <a href="#page<?php print $page['nr']; ?>">Pagina <?php print $page['nr']; ?></a>
                      <ul>
                        <?php foreach ($page['amendments'] as $amendment) : ?>
                          <li>
                            <a href="#amendment<?php print $amendment['id']; ?>">Amendement nr. <?php print $amendment['id']; ?></a>
                          </li>
                        <?php endforeach; ?>
                      </ul>
                    </li>
                  <?php endforeach; ?>
                </ul>
              </li>
            <?php endforeach; ?>
          </ul>
        </li>
      <?php endforeach; ?>
    </ul>

    <ul class="first">
      <?php foreach ($meetings[1]['documents'] as $document) : ?>
        <li>
          <h1 id="document<?php print $document['id']; ?>"><?php print $document['title']; ?></h1>
          <ul>
            <h2 id="algemeen">Algemeen</h2>
            <ul><ul>
              <?php foreach ($document['type']['general']['amendments'] as $amendment) : ?>
                <li>
                  <?php print theme('amendment', array('entity_id' => $amendment['id'], 'destination' => $destination)); ?>
                </li>
              <?php endforeach; ?>
            </ul></ul>
            <?php foreach ($document['type']['specific']['chapters'] as $chapter) : ?>
              <li>
                <h2 id="chapter<?php print $chapter['nr']; ?>">Hoofdstuk <?php print $chapter['nr']; ?></h2>
                <ul>
                  <?php foreach ($chapter['pages'] as $page) : ?>
                    <li>
                      <h2 id="page<?php print $page['nr']; ?>">Pagina <?php print $page['nr']; ?></h2>
                      <ul>
                        <?php foreach ($page['amendments'] as $amendment) : ?>
                          <li>
                            <?php print theme('amendment', array('entity_id' => $amendment['id'], 'destination' => $destination)); ?>
                          </li>
                        <?php endforeach; ?>
                      </ul>
                    </li>
                  <?php endforeach; ?>
                </ul>
              </li>
            <?php endforeach; ?>
          </ul>
        </li>
      <?php endforeach; ?>
    </ul>

  <?php endif; ?>
<?php endif; ?>
<hr/>
