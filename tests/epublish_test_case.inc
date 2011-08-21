<?php

require_once "epublish_classes.inc";

class EPublishTestCase extends DrupalWebTestCase {

    /**
     * This tests to ensure that I've created the links that I expect using the
     * label attribute on them. It's a bit nicer than the text checking.
     */

    public function assertLinkLabel($label, $index = 0, $message = '', $group = 'Other') {
        $links = $this->xpath('//a[@label="' . $label . '"]');
        $message = ($message ?  $message : t('Link with label "!label" found.', array('!label' => $label)));
        return $this->assert(isset($links[$index]), $message, $group);
    }

    /**
     *  These are utility functions for the subclasses to use
     */

    public function Get($url)
    {
        return $this->drupalGet($url);
    }

    public function Post($one, $two, $three)
    {
        return $this->drupalPost($one, $two, $three);
    }

    public function getEpubId($name) {
        $this->drupalGet("admin/epublish");
        $links = $this->xpath('//a[text()="' . $name . '"]');
        foreach($links[0]->attributes() as $n => $v) {
            if ($n == "href") {
                preg_match("/(\d+)$/", $v, $matches);
                return $matches[0];
            }
        }
        return 0;
    }

    public function getEpubEdId($name) {
        $this->drupalGet("admin/epublish");
        if( preg_match("/-$name.*?\(<a href=\".*?(\d+)\">edit edition/", $this->content, $matches) ) {
            return $matches[1];
        } else {
            return 0;
        }
    }

}
?>