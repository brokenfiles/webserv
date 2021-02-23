var nav = document.getElementById('header-nav')
var burger = document.getElementById('burger')
var header = document.getElementById('header')
var logo = document.getElementById('logo')

var onresize = function() {
    width = document.body.clientWidth;
    if (width < 1300) {
        nav.style.display = 'none'
        burger.style.display = 'block'
        header.classList.add('header-responsive')
        logo.style.paddingLeft = '40px'
        burger.style.marginRight = '20px'
    }
    else
    {
        nav.style.display = 'block';
        burger.style.display = 'none'
        logo.style.paddingLeft = '0px'
        burger.style.marginRight = '0px'
        header.classList.remove('header-responsive')
    }

}
window.addEventListener("resize", onresize);

if (window.innerWidth < 1300)
{
    onresize();
}
