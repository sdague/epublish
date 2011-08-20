<?php

class EpubPub {
    protected $test;
    public $id;
    public $name;
    public $description;
    public $eds;

    public function __construct($testclass = NULL) {
        $this->test = $testclass;
        $this->eds = array();
    }

    public function formData() {
        $data = array();
        $data['name'] = $this->name;
        $data['description'] = $this->description;
        // TODO: we shouldn't need these, but apparently we do
        $data['layout_page'] = '0';
        $data['layout_list'] = '0';
        $data['sid'] = '0';
        return $data;
    }

    public function createRandom() {
        $test = $this->test;
        $test->Get('admin/epublish/add/publication');

        $this->name = $test->randomName(8);
        $this->description = $test->randomString(50);

        $test->Post(NULL, $this->formData(), t('Submit'));
        $this->id = $test->getEpubId($this->name);
    }

    public function editLink() {
        return "admin/epublish/edit/publication/" . $this->id;
    }

    public function viewLink() {
        return "epublish/" . $this->id;
    }

    public function createRandomEd() {
        $num = count($this->eds) + 1;
        $ed = new EpubEdition($this->test);
        $ed->createRandom($this->id, $num);
        $this->eds[] = $ed;
        return $ed;
    }

    public function deleteEd($eid) {
        $neweds = array();
        $ret = NULL;
        foreach ($this->eds as $ed) {
            if ($ed->id == $eid) {
                $ret = $ed;
            } else {
                $neweds[] = $ed;
            }
        }
        $ret->delete();
        $this->eds = $neweds;
        return $ret;
    }

}

class EpubEdition {
    protected $test;

    public $id;
    public $pid;
    public $dateline;
    public $volume;
    public $number;
    public $description;
    public $sid;
    public $articles;

    public function __construct($testclass = NULL) {
        $this->test = $testclass;
    }

    public function createRandom($pid, $num) {
        $test = $this->test;
        $test->Get("admin/epublish/add/edition/$pid");

        $this->pid = $pid;
        $this->dateline = $test->randomName(20);
        $this->volume = '2011';
        $this->number = $num;
        $this->description = $test->randomString(100);
        $this->sid = 1;

        $test->Post(NULL, $this->formData(), t('Submit'));
        $this->id = $test->getEpubEdId($this->dateline);
    }

    public function editLink() {
        return "admin/epublish/edit/edition/" . $this->id;
    }

    public function articlesLink() {
        return "admin/epublish/headlines/" . $this->id;
    }

    public function formData() {
        $data = array();
        // $data['pid'] = $this->pid;
        $data['dateline'] = $this->dateline;
        $data['volume'] = $this->volume;
        $data['number'] = $this->number;
        $data['description'] = $this->description;
        $data['sid'] = '0';
        return $data;
    }

    public function delete() {
        $test = $this->test;
        $test->Get($this->editLink());
        // twice because of the confirmation screen
        $test->Post(NULL, array(), t("Delete"));
        $test->Post(NULL, array(), t("Delete"));
    }

}

?>