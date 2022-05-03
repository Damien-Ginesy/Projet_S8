<?php
$data = file_get_contents("php://input");
if($data){
    echo $data;
}
else http_response_code(400);
?>