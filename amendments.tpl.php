<h1 id="inhoud">Overzicht alle ingediende amendementen</h1>
<?php if (!empty($meeting['documents'])) : ?>

  <ul class="first navigation">
    <?php foreach ($meeting['documents'] as $document) : ?>
      <li class="document">
        <a href="#document<?php print $document['id']; ?>"><strong><?php print $document['title']; ?></strong></a>
        <ul>
          Amendement index
          <ul class="chapters">
            <?php foreach ($document['amendment_index'] as $chapter => $amendment_ids) : ?>
              <li>
                <ul class="amendments">
                <?php foreach ($amendment_ids as $amendment_id) : ?>
                  <?php $amendment = $document['amendments'][$amendment_id]; ?>
                  <li>
                    <a href="#amendment<?php print $amendment['id']; ?>">#<?php print (!empty($chapterized_id)) ? $amendment['chapter'] . '.' . $amendment['chapterized_id'] : $amendment_id; ?></a>
                  </li>
                <?php endforeach; ?>
                </ul>
              </li>
            <?php endforeach; ?>
          </ul>
          <?php if (!empty($document['chapters'])): ?>
            <?php foreach ($document['chapters'] as $chapter) : ?>
              <li class="chapter">
                <a href="#chapter<?php print $chapter['nr']; ?>">Hoofdstuk <?php print $chapter['nr']; ?></a>
                <ul>
                  <?php foreach ($chapter['pages'] as $page) : ?>
                    <li>
                      <span class="label"><a href="#page<?php print $page['nr']; ?>">Pagina <?php print $page['nr']; ?></a></span>
                      <!--
                      <ul class="amendments">
                        <?php foreach ($page['amendments'] as $amendment_id) : ?>
                          <?php $amendment = $document['amendments'][$amendment_id]; ?>
                          <li>
                            <a href="#amendment<?php print $amendment['id']; ?>">#<?php print $amendment['chapter'] . '.' . $amendment['chapterized_id']; ?></a>
                          </li>
                        <?php endforeach; ?>
                      </ul>
                      -->
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
        <h2 id="document<?php print $document['id']; ?>" class="document-title"><?php print $document['title']; ?></h2>
        <ul>
          <?php if (!empty($document['chapters'])): ?>
            <?php foreach ($document['chapters'] as $chapter) : ?>
              <li>
                <h3 id="chapter<?php print $chapter['nr']; ?>" class="chapter-nr">Hoofdstuk <?php print $chapter['nr']; ?></h3>
                <ul>
                  <?php foreach ($chapter['pages'] as $page) : ?>
                    <li>
                      <h3 id="page<?php print $page['nr']; ?>" class="page-nr">Pagina <?php print $page['nr']; ?></h2>
                      <ul>
                        <?php foreach ($page['amendments'] as $amendment_id) : ?>
                          <?php $amendment = $document['amendments'][$amendment_id]; ?>
                          <li class="ammo-element <?php print $amendment['state']; ?>">
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
