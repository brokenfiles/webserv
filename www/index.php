<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <title>Webserv</title>
    <link rel="stylesheet" href="style.css">
</head>
<body>
<header id="header" class="header">
    <logo id="logo">Webserv</logo>
    <div id="header-nav" class="header-nav">
        <a href="#">Qui sommes-nous ?</a>
        <a href="#">Contact</a>
        <a href="#">Connexion</a>
    </div>
    <div id="burger" class="burger">
        <span class="burger-span"></span>
    </div>
</header>
<div class="content">
	<?php var_dump($_POST) ?>
    <h5 class="titre"><?php echo 'post= ' . $_POST["name"]?></h5>
    <p><?php var_dump(getenv())?></p>
</div>
<script type="text/javascript" src="app.js"></script>
</body>
</html>
