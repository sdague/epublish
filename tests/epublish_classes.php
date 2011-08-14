<?php

class EpubPub {
    protected $test;
    public $id;
    public $name;
    public $description;

    public function __construct($testclass = NULL) {
        $this->test = $testclass;
    }

    public function formData() {
        $data = array();
        $data['name'] = $this->name;
        $data['description'] = $this->description;
        return $data;
    }

    public function createRandom() {
        $test = $this->test;
        $this->name = $test->randomName(8);
        $this->description = $test->randomString(50);

        $test->Post('admin/epublish/add/publication', $this->formData(), t('Submit'));
        $this->id = $test->getEpubId($this->name);
    }

    public function editLink() {
        return "admin/epublish/edit/publication/" . $this->id;
    }

    public function viewLink() {
        return "epublish/" . $this->id;
    }

}

?>