// 订单页面
package frame;

import javax.swing.*;
import javax.swing.table.DefaultTableModel;

import pojo1.*;
import pojo2.*;

import java.awt.*;
import java.awt.event.*;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;

public class OrderFrame extends JFrame implements ActionListener {
    private JLabel nameLabel, emailLabel, addressLabel, phoneLabel, totalLabel, orderLabel;
    private JTextField nameField, emailField, addressField, phoneField, totalField, orderField;
    private JTable table;
    private JButton backButton, confirmButton;
    private JScrollPane scrollPane;
    private DefaultTableModel tableModel;
    String text;
    User user = null;
    Trolley tro = null;
    boolean flag;
    private ArrayList<Product> products = Product.fromFile("txt\\Products.txt");
    ArrayList<Object[]> dataList = new ArrayList<Object[]>();

    public OrderFrame(User user, Trolley tro, boolean flag, String txt) {
        this.user = user;
        this.tro = tro;
        this.flag = flag;
        setTitle("订单界面");
        setSize(1000, 800); 
        setResizable(true); // 改变窗口大小
        setLocationRelativeTo(null); // 将窗口位置设置为屏幕中央
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

        // 用户信息面板
        JPanel userPanel = new JPanel(new GridLayout(4, 2));

        nameLabel = new JLabel("用户名：");
        nameLabel.setFont(new Font("宋体", Font.PLAIN, 30));
        nameField = createTextField(user.getUsername());

        emailLabel = new JLabel("邮箱：");
        emailLabel.setFont(new Font("宋体", Font.PLAIN, 30));
        emailField = createTextField(user.getEmail());

        addressLabel = new JLabel("地址：");
        addressLabel.setFont(new Font("宋体", Font.PLAIN, 30));
        addressField = createTextField(user.getAddress());

        phoneLabel = new JLabel("电话：");
        phoneLabel.setFont(new Font("宋体", Font.PLAIN, 30));
        phoneField = createTextField(user.getPhonenum());

        userPanel.add(nameLabel);
        userPanel.add(nameField);
        userPanel.add(emailLabel);
        userPanel.add(emailField);
        userPanel.add(addressLabel);
        userPanel.add(addressField);
        userPanel.add(phoneLabel);
        userPanel.add(phoneField);

        tableModel = new DefaultTableModel();
        // 商品列表面板
        JPanel productPanel = new JPanel(new BorderLayout());
        String[] columnNames = { "商品编号", "名称", "类别", "数量", "单价" };
        if (tro != null) {
            for (Product product : tro.getProlist()) {
                dataList.add(new Object[] { product.getId(), product.getName(),
                        product.getCategory(), product.getAmount(), product.getPrice() });
            }
        }

        // 将ArrayList数据转换为二维数组
        Object[][] data = dataList.toArray(new Object[dataList.size()][]);
        tableModel.setDataVector(data, columnNames);
        table = new JTable(data, columnNames);
        table.setFont(new Font("宋体", Font.PLAIN, 20));
        table.getColumnModel().getColumn(0).setPreferredWidth(100); // 将第一列的宽度设置为100像素
        table.getColumnModel().getColumn(1).setPreferredWidth(200); // 将第二列的宽度设置为200像素
        table.setRowHeight(30); // 将行高设置为30像素
        scrollPane = new JScrollPane(table);
        productPanel.add(scrollPane, BorderLayout.CENTER);

        // 订单信息面板
        JPanel orderPanel = new JPanel(new GridLayout(5, 5));

        // 添加标签和文本框
        totalLabel = new JLabel("总价：");
        totalLabel.setFont(new Font("宋体", Font.PLAIN, 30));
        orderPanel.add(totalLabel);
        totalField = new JTextField(20);
        totalField.setFont(new Font("宋体", Font.PLAIN, 30));
        totalField.setEditable(false);
        orderPanel.add(totalField);

        orderLabel = new JLabel("订单号：");
        orderLabel.setFont(new Font("宋体", Font.PLAIN, 30));
        orderPanel.add(orderLabel);
        orderField = new JTextField(20);
        orderField.setFont(new Font("宋体", Font.PLAIN, 30));
        orderField.setEditable(false);
        orderPanel.add(orderField);

        confirmButton = new JButton("确认");
        confirmButton.setFont(new Font("宋体", Font.PLAIN, 30));
        confirmButton.addActionListener(this);
        orderPanel.add(confirmButton);

        backButton = new JButton("返回");
        backButton.setFont(new Font("宋体", Font.PLAIN, 30));
        backButton.addActionListener(this);
        orderPanel.add(backButton);

        // 添加面板到窗口
        add(userPanel, BorderLayout.NORTH);
        add(productPanel, BorderLayout.CENTER);
        add(orderPanel, BorderLayout.SOUTH);

        double totalPrice = 0;
        for (int i = 0; i < tableModel.getRowCount(); i++) {
            int amount = Integer.parseInt(tableModel.getValueAt(i, 3).toString());
            double price = (double) tableModel.getValueAt(i, 4);
            totalPrice += amount * price;
            if (amount == 0) {
                dataList.remove(i);
            }
        }
        totalField.setText(String.format("%.2f", totalPrice));

        // 生成订单号
        if (txt == null) {
            this.text = Order.randomID();
        } else {
            this.text = txt;
        }

        orderField.setText(this.text);
        setVisible(true);
    }

    private JTextField createTextField(String text) {
        JTextField textField = new JTextField(text);
        textField.setFont(new Font("宋体", Font.PLAIN, 25));
        textField.setPreferredSize(new Dimension(200, 40));
        return textField;
    }

    public void actionPerformed(ActionEvent e) {
        if (e.getSource() == confirmButton) {
            if (flag) {
                if (tro != null) {
                    for (Product product : tro.getProlist()) {
                        products.get(product.getId()-1)
                                .setAmount(products.get(product.getId()).getAmount() - product.getAmount());
                    }
                }
                try {
                    FileWriter fw = new FileWriter("txt\\Products.txt");
                    fw.write("");
                    fw.close();
                } catch (IOException er) {
                    System.out.println("发生错误: " + er.getMessage());
                }
                for (Product product : products) {
                    product.toFile("txt\\Products.txt", true);
                }
                Order ord = new Order(tro, text);
                ord.toFile("txt\\Orders.txt", flag);
                tro.getProlist().clear();
                dispose();
                new TrolleyFrame(user, tro);
            } else {
                dispose();
                new OrderTurn(user);
            }
        } else if (e.getSource() == backButton) {
            if (flag) {
                dispose();
                new TrolleyFrame(user, tro);
            } else {
                dispose();
                new OrderTurn(user);
            }
        }
    }
}