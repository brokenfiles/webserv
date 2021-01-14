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
    <h1 class="titre"><?php echo 'QUERY_STRING=' . getenv('QUERY_STRING')?></h1>
    <h1 class="titre"><?php echo 'get= ' . $_GET["salut"]?></h1>
</div>
<script type="text/javascript" src="app.js"></script>
</body>
</html>