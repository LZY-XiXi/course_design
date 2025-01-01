// 主页面
package frame;

import javax.swing.*;

import pojo1.User;
import pojo2.Trolley;

import java.awt.*;
import java.awt.event.*;

public class MainFrame extends JFrame implements ActionListener {
    private JButton productButton, cartButton, orderButton, userButton, backButton;
    Trolley tro = new Trolley(null);
    User u;

    public MainFrame(User u, Trolley tro) {
        this.u = u;
        this.tro = tro;
        // 设置窗口
        setTitle("购物系统入口");
        setSize(1000, 800);
        setResizable(true); // 改变窗口大小
        setLocationRelativeTo(null); // 将窗口定位到屏幕中央
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

        // 创建面板并设置布局
        JPanel panel = new JPanel();
        panel.setLayout(new GridLayout(5, 8, 100, 50));

        // 添加按钮
        productButton = createButton("商品界面");
        panel.add(productButton);

        cartButton = createButton("购物车界面");
        panel.add(cartButton);

        orderButton = createButton("订单界面");
        panel.add(orderButton);

        userButton = createButton("修改用户信息");
        panel.add(userButton);

        backButton = createButton("返回");
        panel.add(backButton);

        // 将面板添加到窗口中
        add(panel);

        // 显示窗口
        setVisible(true);
    }

    private JButton createButton(String text) {
        JButton button = new JButton(text);
        button.setFont(new Font("宋体", Font.PLAIN, 30));
        button.addActionListener(this);
        return button;
    }

    @Override
    public void actionPerformed(ActionEvent e) {
        if (e.getSource() == productButton) {
            dispose();
            new ProductFrame(u);
        } else if (e.getSource() == cartButton) {
            dispose();
            new TrolleyFrame(u, tro);
        } else if (e.getSource() == orderButton) {
            dispose();
            new OrderTurn(u);
        } else if (e.getSource() == userButton) {
            dispose();
            new UsersFrame(u);
        } else if (e.getSource() == backButton) {
            dispose();
            new LoginFrame();
        }
    }
}
