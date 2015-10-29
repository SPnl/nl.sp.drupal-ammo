<h1 id="inhoud">Inhoud</h1>
<?php if (!empty($meeting['documents'])) : ?>

  <ul class="first">
    <?php foreach ($meeting['documents'] as $document) : ?>
      <li>
        <a href="#document<?php print $document['id']; ?>"><?php print $document['title']; ?></a>
        <ul>
          <?php if (!empty($document['chapters'])): ?>
            <?php foreach ($document['chapters'] as $chapter) : ?>
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
          <?php endif; ?>
        </ul>
      </li>
    <?php endforeach; ?>
  </ul>

  <ul class="first">
    <?php foreach ($meeting['documents'] as $document) : ?>
      <li>
        <h1 id="document<?php print $document['id']; ?>"><?php print $document['title']; ?></h1>
        <ul>
          <?php if (!empty($document['chapters'])): ?>
            <?php foreach ($document['chapters'] as $chapter) : ?>
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
          <?php endif; ?>
        </ul>
      </li>
    <?php endforeach; ?>
  </ul>

<?php endif; ?>
<hr/>
