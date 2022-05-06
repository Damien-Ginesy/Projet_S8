<?php
$data = file_get_contents("php://input");
if($data){

    $parsed_data = json_decode($data, true);
    $id = $parsed_data[0]["nodeID"]["adresseVirtuelle"];

    $bdd = json_decode(file_get_contents("log.log"), true);

    $bdd[intval($id)] = $parsed_data[0];

    file_put_contents("log.log", json_encode($bdd));

}
?>
