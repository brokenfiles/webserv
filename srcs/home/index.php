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
    <h5 class="titre"><?php echo 'get= ' . $_GET["salut"]?></h5>
	<h5><?= var_dump(getenv())?></h5>
</div>
<script type="text/javascript" src="app.js"></script>
</body>
</html>
