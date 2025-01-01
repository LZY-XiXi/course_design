// 用户界面
package frame;

import pojo2.Trolley;

import java.util.ArrayList;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JTextField;

import pojo1.*;

import java.awt.*;
import java.awt.event.*;
import java.io.FileWriter;
import java.io.IOException;

public class UsersFrame extends JFrame implements ActionListener {
    private JTextField usernameField, emailField, phoneField, addressField, passwordField;
    private JButton registerButton, exitButton;
    private User u;
    private Trolley tro = null;

    private ArrayList<User> users = User.fromFile("txt\\Users.txt");

    public UsersFrame(User u) {
        // 初始化
        this.u = u;
        // 设置窗口
        setTitle("用户信息");
        setSize(1000, 800);
        setResizable(false); // 不能改变窗口大小
        setLocationRelativeTo(null); // 将窗口定位到屏幕中央
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

        // 创建面板并设置布局
        JPanel panel = new JPanel();
        panel.setLayout(new GridLayout(6, 8, 100, 50));

        // 添加标签输入框
        JLabel usernameLabel = new JLabel("用户名:");
        usernameLabel.setFont(new Font("宋体", Font.PLAIN, 30));
        panel.add(usernameLabel);
        usernameField = createTextField(u.getUsername());
        panel.add(usernameField);

        JLabel emailLabel = new JLabel("邮箱:");
        emailLabel.setFont(new Font("宋体", Font.PLAIN, 30));
        panel.add(emailLabel);
        emailField = createTextField(u.getEmail());
        panel.add(emailField);

        JLabel phoneLabel = new JLabel("电话号码:");
        phoneLabel.setFont(new Font("宋体", Font.PLAIN, 30));
        panel.add(phoneLabel);
        phoneField = createTextField(u.getPhonenum());
        panel.add(phoneField);

        JLabel addressLabel = new JLabel("地址:");
        addressLabel.setFont(new Font("宋体", Font.PLAIN, 30));
        panel.add(addressLabel);
        addressField = createTextField(u.getAddress());
        panel.add(addressField);

        JLabel passwordLabel = new JLabel("密码:");
        passwordLabel.setFont(new Font("宋体", Font.PLAIN, 30));
        panel.add(passwordLabel);
        passwordField = createTextField(u.getPassword());
        panel.add(passwordField);

        // 添加按钮
        registerButton = new JButton("确认");
        registerButton.setFont(new Font("宋体", Font.PLAIN, 30));
        registerButton.addActionListener(this);
        panel.add(registerButton);

        exitButton = new JButton("返回");
        exitButton.setFont(new Font("宋体", Font.PLAIN, 30));
        exitButton.addActionListener(this);
        panel.add(exitButton);

        // 将面板添加到窗口中
        add(panel);

        // 让窗口全屏显示
        setExtendedState(JFrame.MAXIMIZED_BOTH);

        // 显示窗口
        setVisible(true);
    }

    private JTextField createTextField(String text) {
        JTextField textField = new JTextField(text);
        textField.setFont(new Font("宋体", Font.PLAIN, 25));
        textField.setPreferredSize(new Dimension(200, 40));
        return textField;
    }

    @Override
    public void actionPerformed(ActionEvent e) {
        if (e.getSource() == registerButton) {
            // 获取用户输入的注册信息
            String username = usernameField.getText();
            String email = emailField.getText();
            String phone = phoneField.getText();
            String address = addressField.getText();
            String password = passwordField.getText();

            int id = u.getId();

            u.setUsername(username);
            u.setEmail(email);
            u.setPhonenum(phone);
            u.setAddress(address);
            u.setPassword(password);
            User user = new User(id, username, email, phone, address, password);

            users.set(id-1, user);

            JOptionPane.showMessageDialog(this, "修改成功", "提示", JOptionPane.INFORMATION_MESSAGE);
            try {
                FileWriter fw = new FileWriter("txt\\Users.txt");
                fw.write("");
                fw.close();
            } catch (IOException er) {
                System.out.println("发生错误: " + er.getMessage());
            }
            for (User us : users) {
                us.toFile("txt\\Users.txt", true);
            }
            dispose();
            new UsersFrame(u);

        } else if (e.getSource() == exitButton) {
            // 退出程序
            dispose();
            new MainFrame(u, tro);
        }
    }
}