BEGIN;

-- lid, the link ID, is linked to the sequence links_lid
CREATE TABLE links (
  lid INT4 NOT NULL DEFAULT '0',
  url_md5 varchar(32) NOT NULL DEFAULT '',
  url TEXT DEFAULT '' NOT NULL,
  link_title varchar(255) NOT NULL DEFAULT '',
  last_click_time INT4 NOT NULL DEFAULT '0',
  PRIMARY KEY (lid)
);

-- lid is a foreign key to links
CREATE TABLE links_monitor (
  lid INT4 NOT NULL DEFAULT '0',
  check_interval INT4 NOT NULL DEFAULT '0',
  last_check INT4 NOT NULL DEFAULT '0',
  fail_count INT4 NOT NULL DEFAULT '0',
  alternate_monitor_url text,
  redirect_propose_url text,
  redirect_saved_url text,
  change_threshold INT4 NOT NULL DEFAULT '0',
  change_flag INT4 NOT NULL DEFAULT '0',
  change_last_data text,
  PRIMARY KEY (lid)
  );

-- lid is a foreign key to links
-- The interval defaults to 1 week
CREATE TABLE links_node (
  lid INT4 NOT NULL DEFAULT '0',
  nid INT4 NOT NULL DEFAULT '0',
  link_title varchar(255) NOT NULL DEFAULT '',
  weight INT4 NOT NULL DEFAULT '0',
  clicks INT4 NOT NULL DEFAULT '0',
  module varchar(100) NOT NULL DEFAULT '',
  PRIMARY KEY (lid,nid,module)
);

COMMIT;
