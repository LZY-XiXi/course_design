//游戏环节
var timeout1 = setInterval(back, 60000);
var timeout2 = setInterval(automove, 5000);
var game = {
    data: [],
    score: 0,   // 初始分数
    status: 1,  // 游戏状态
    // 游戏开始时还原所有
    start: function () {
        this.data = [
            [0, 0, 0, 0],
            [0, 0, 0, 0],
            [0, 0, 0, 0],
            [0, 0, 0, 0]
        ];
        this.score = 0;
        this.status = 1;
        // 开局随机生成5个2或者4
        for (var i = 0; i < 5; i++) {
            this.randomNum();
        }
        this.dataView(); // 更新视图
    },
    //随机赋值
    randomNum: function () {
        //检测空白位置
        while (true) {
            // 随机行列
            var r = Math.floor(Math.random() * 4);
            var c = Math.floor(Math.random() * 4);
            // 判断 
            if (this.data[r][c] == 0) {
                var num = Math.random() > 0.3 ? 2 : 4; // 随机生成2或者4
                this.data[r][c] = num;
                break;

            }

        }
    },
    //更新视图的方法
    dataView: function () {
        // 直接循环
        for (var r = 0; r < 4; r++) {
            for (var c = 0; c < 4; c++) {
                var div = document.getElementById("n" + r + c);
                if (this.data[r][c] != 0) {
                    div.innerHTML = this.data[r][c];
                    div.className = "cell n" + this.data[r][c];
                } else {
                    div.innerHTML = "";  // 等于0的时候，div里面的内容直接置空
                    div.className = "cell";
                }
            }
        }
        // 更新分数
        document.getElementById("score01").innerHTML = this.score;
        // 判断游戏是否结束
        if (this.status == 0) {  // 游戏结束弹出框显示
            document.getElementById("gameover").style.display = "block";
            document.getElementById("score02").innerHTML = this.score;
        } else {
            document.getElementById("gameover").style.display = "none";
        }
    },
    //	判断游戏是否结束的方法
    isgameover: function () {
        for (var r = 0; r < 4; r++) {
            for (var c = 0; c < 4; c++) {
                if (this.data[r][c] == 0) {  //有空白的时候
                    return false;
                }
                if (c < 3) {  //判断左右是否有相同
                    if (this.data[r][c] == this.data[r][c + 1]) {
                        return false;
                    }
                }
                if (r < 3) {
                    if (this.data[r][c] == this.data[r + 1][c]) {
                        return false;
                    }
                }
            }
        }
        return true;   //上面情况全都不符合，表示游戏已经结束
    },

    //数字左移
    moveLeft: function () {
        var before = String(this.data);
        for (var r = 0; r < 4; r++) {
            this.moveLeftinRow(r);
        }
        var after = String(this.data);
        if (before != after) {
            this.randomNum();
            if (this.isgameover()) { //如果游戏结束,自己状态等于游戏结束状态
                this.status = 0;
            }
            //更新视图
            this.dataView();
        }

    },
    //处理每一行的数据
    moveLeftinRow: function (r) {
        //循环获取后面的数据,最左边不用考虑
        for (var c = 0; c < 3; c++) {
            var nextc = this.moveLeftNum(r, c);
            //判断是否为-1，否则则为找到数字
            if (nextc != -1) {
                if (this.data[r][c] == 0) {  //如果当前的数等于0，则当前的数和找到的数进行比较
                    this.data[r][c] = this.data[r][nextc];
                    //找到的数清空变为0
                    this.data[r][nextc] = 0;
                    //再次从最左边的数进行循环
                    c--;

                }
                else if (this.data[r][c] == this.data[r][nextc]) {
                    //如果当前的数等于找到的数，则相加
                    this.data[r][c] *= 2;
                    //找到的数清空变为0
                    this.data[r][nextc] = 0;
                    //数字相加一次分数加
                    this.score += this.data[r][c];

                }
            } else {
                //如果没有找到数，则退出循环
                break;
            }
        }

    },
    moveLeftNum: function (r, c) {	//	左移
        for (var i = c + 1; i < 4; i++) {
            //判断后面是否找到数字
            if (this.data[r][i] != 0) {
                return i;
            }
        }
        //如果没有找到，返回
        return -1;
    },

    //数字右移
    moveRight: function () {
        var before = String(this.data);
        for (var r = 0; r < 4; r++) {
            this.moveRightinRow(r);
        }
        var after = String(this.data);
        if (before != after) {
            this.randomNum();
            if (this.isgameover()) { //如果游戏结束,自己状态等于游戏结束状态
                this.status = 0;
            }
            //更新视图
            this.dataView();
        }

    },
    //处理每一行的数据
    moveRightinRow: function (r) {
        //循环获取前面的数据,最左边不用考虑
        for (var c = 3; c >= 0; c--) {
            //变量接收
            var nextc = this.moveRightNum(r, c);
            //判断是否为-1，否则则为找到数字
            if (nextc != -1) {
                if (this.data[r][c] == 0) {  //如果当前的数等于0，则当前的数和找到的数进行比较
                    this.data[r][c] = this.data[r][nextc];
                    //找到的数清空变为0
                    this.data[r][nextc] = 0;
                    //再次从最右边的数进行循环
                    c++;

                }
                else if (this.data[r][c] == this.data[r][nextc]) {
                    //如果当前的数等于找到的数，则相加
                    this.data[r][c] *= 2;
                    //找到的数清空变为0
                    this.data[r][nextc] = 0;
                    //数字相加一次分数加
                    this.score += this.data[r][c];

                }
            } else {
                //如果没有找到数，则退出循环
                break;
            }
        }

    },
    moveRightNum: function (r, c) {	//	左移
        //循环获取前面的数据,最右边不用考虑
        for (var i = c - 1; i >= 0; i--) {
            //判断前面是否找到数字
            if (this.data[r][i] != 0) {
                //返回下标
                return i;
            }
        }
        //如果没有找到，返回
        return -1;
    },

    //数字上移
    moveUp: function () {
        var before = String(this.data);
        for (var c = 0; c < 4; c++) {
            this.moveUpinRow(c);
        }
        var after = String(this.data);
        if (before != after) {
            this.randomNum();
            if (this.isgameover()) { //如果游戏结束,自己状态等于游戏结束状态
                this.status = 0;
            }
            //更新视图
            this.dataView();
        }
    },
    //处理每一行的数据
    moveUpinRow: function (c) {
        for (var r = 0; r < 3; r++) {
            var nextr = this.moveUpNum(r, c);
            if (nextr != -1) {
                if (this.data[r][c] == 0) { 
                    this.data[r][c] = this.data[nextr][c];
                    this.data[nextr][c] = 0;
                    r--;
                }
                else if (this.data[r][c] == this.data[nextr][c]) {
                    this.data[r][c] *= 2;
                    this.data[nextr][c] = 0;
                    this.score += this.data[r][c];
                }
            } else {
                break;
            }
        }
    },
    moveUpNum: function (r, c) {	//	上移
        //循环获取上面的数据,最右边不用考虑
        for (var i = r + 1; i < 4; i++) {
            //判断下面是否找到数字
            if (this.data[i][c] != 0) {
                //返回下标
                return i;
            }
        }
        //如果没有找到，返回
        return -1;
    },

    //数字下移
    moveDown: function () {
        var before = String(this.data);
        for (var c = 0; c < 4; c++) {
            this.moveDowninRow(c);
        }
        var after = String(this.data);
        if (before != after) {
            this.randomNum();
            if (this.isgameover()) { //如果游戏结束,自己状态等于游戏结束状态
                this.status = 0;
            }
            //更新视图
            this.dataView();
        }
    },
    //处理每一行的数据
    moveDowninRow: function (c) {
        //循环获取前面的数据,最下面不用考虑
        for (var r = 3; r >= 0; r--) {
            var nextr = this.moveDownNum(r, c);
            if (nextr != -1) {
                if (this.data[r][c] == 0) {  //如果当前的数等于0，则当前的数和找到的数进行比较
                    this.data[r][c] = this.data[nextr][c];
                    this.data[nextr][c] = 0;
                    r++;
                }
                else if (this.data[r][c] == this.data[nextr][c]) {
                    this.data[r][c] *= 2;
                    this.data[nextr][c] = 0;
                    this.score += this.data[r][c];
                }
            } else {
                break;
            }
        }

    },
    moveDownNum: function (r, c) {	//	下移
        for (var i = r - 1; i >= 0; i--) {
            if (this.data[i][c] != 0) {
                return i;
            }
        }
        return -1;
    }

}

//点击再来一次
function typeonce() {
    document.getElementById("gameover").style.display = "none";
    window.location.href = "../game";
}
// 返回main页面
function back() {
    alert("这么久不动? 那就是不想玩了,先回去吧");
    window.location.href = "../main";
}
// 随机执行一步操作
function automove() {
    var ran = Math.floor(Math.random() * 4);

    switch (ran) {
        case 0:
            game.moveDown();
            break;
        case 1:
            game.moveUp();
            break;
        case 2:
            game.moveLeft();
            break;
        case 3:
            game.moveRight();
            break;
    }
}

//键盘事件
document.onkeydown = function (event) {
    clearInterval(timeout1);
    clearInterval(timeout2);
    timeout1 = setInterval(back, 60000)
    timeout2 = setInterval(automove, 5000)
    if (event.key == "ArrowLeft") {
        game.moveLeft(); // 左移
    } else if (event.key == "ArrowUp") {
        game.moveUp(); // 上移
    } else if (event.key == "ArrowRight") {
        game.moveRight(); // 右移
    } else if (event.key == "ArrowDown") {
        game.moveDown(); // 下移
    }
}

//游戏开始时调用
game.start();
