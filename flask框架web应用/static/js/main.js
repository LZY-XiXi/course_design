// 导航栏
function showContent(section) {
    var sections = ['纸嫁衣简介', '下载地址', '这里什么也没有',];
    for (var i = 0; i < sections.length; i++) {
        document.getElementById(sections[i]).style.display = 'none';
    }
    document.getElementById(section).style.display = 'block';
}
showContent('纸嫁衣简介');  
