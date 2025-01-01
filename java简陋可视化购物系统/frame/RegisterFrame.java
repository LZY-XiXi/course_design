// 注册页面
package frame;

import java.util.List;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JPasswordField;
import javax.swing.JTextField;

import pojo1.*;

import java.awt.*;
import java.awt.event.*;

public class RegisterFrame extends JFrame implements ActionListener {
    private JTextField usernameField, emailField, phoneField, addressField;
    private JPasswordField passwordField;
    private JButton registerButton, exitButton;
    private List<User> userlist = User.fromFile("txt\\Users.txt");

    public RegisterFrame() {
        // 设置窗口
        setTitle("注册窗口");
        setSize(1000, 800);
        setResizable(false); // 不能改变窗口大小
        setLocationRelativeTo(null); // 将窗口定位到屏幕中央
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

        // 创建面板并设置布局
        JPanel mainPanel = new JPanel();
        mainPanel.setLayout(new BorderLayout());

        JPanel userNumberPanel = new JPanel();
        JLabel userNumberLabel = new JLabel("当前用户编号：" + (userlist.size()+1));
        userNumberLabel.setFont(new Font("宋体", Font.PLAIN, 30));
        userNumberPanel.add(userNumberLabel);

        mainPanel.add(userNumberPanel, BorderLayout.NORTH);

        JPanel inputPanel = new JPanel();
        inputPanel.setLayout(new GridLayout(6, 2, 100, 50));

        // 添加标签和输入框
        JLabel usernameLabel = new JLabel("用户名:");
        usernameLabel.setFont(new Font("宋体", Font.PLAIN, 30));
        inputPanel.add(usernameLabel);
        usernameField = new JTextField(20);
        usernameField.setFont(new Font("宋体", Font.PLAIN, 30));
        inputPanel.add(usernameField);

        JLabel emailLabel = new JLabel("邮箱:");
        emailLabel.setFont(new Font("宋体", Font.PLAIN, 30));
        inputPanel.add(emailLabel);
        emailField = new JTextField(20);
        emailField.setFont(new Font("宋体", Font.PLAIN, 30));
        inputPanel.add(emailField);

        JLabel phoneLabel = new JLabel("电话号码:");
        phoneLabel.setFont(new Font("宋体", Font.PLAIN, 30));
        inputPanel.add(phoneLabel);
        phoneField = new JTextField(20);
        phoneField.setFont(new Font("宋体", Font.PLAIN, 30));
        inputPanel.add(phoneField);

        JLabel addressLabel = new JLabel("地址:");
        addressLabel.setFont(new Font("宋体", Font.PLAIN, 30));
        inputPanel.add(addressLabel);
        addressField = new JTextField(20);
        addressField.setFont(new Font("宋体", Font.PLAIN, 30));
        inputPanel.add(addressField);

        JLabel passwordLabel = new JLabel("密码:");
        passwordLabel.setFont(new Font("宋体", Font.PLAIN, 30));
        inputPanel.add(passwordLabel);
        passwordField = new JPasswordField(20);
        passwordField.setFont(new Font("宋体", Font.PLAIN, 30));
        inputPanel.add(passwordField);

        // 添加注册和退出按钮
        registerButton = new JButton("注册");
        registerButton.setFont(new Font("宋体", Font.PLAIN, 30));
        registerButton.addActionListener(this);
        inputPanel.add(registerButton);

        exitButton = new JButton("返回");
        exitButton.setFont(new Font("宋体", Font.PLAIN, 30));
        exitButton.addActionListener(this);
        inputPanel.add(exitButton);

        mainPanel.add(inputPanel, BorderLayout.CENTER);

        // 将面板添加到窗口中
        add(mainPanel);

        // 让窗口全屏显示
        setExtendedState(JFrame.MAXIMIZED_BOTH);

        // 显示窗口
        setVisible(true);
    }

    @Override
    public void actionPerformed(ActionEvent e) {
        if (e.getSource() == registerButton) {
            // 获取用户输入的注册信息
            String username = usernameField.getText();
            String email = emailField.getText();
            String phone = phoneField.getText();
            String address = addressField.getText();
            String password = new String(passwordField.getPassword());

            User u = new User(userlist.size() + 1, username, email, phone, address, password);
            userlist.add(u);
            u.toFile("txt\\Users.txt", true);

            // 显示注册成功消息框并退出
            JOptionPane.showMessageDialog(this, "注册成功！" + "当前用户id为：" + Integer.toString(userlist.size()));
            dispose();
            new LoginFrame();
        } else if (e.getSource() == exitButton) {
            // 退出程序
            dispose();
            new LoginFrame();
        }
    }
}