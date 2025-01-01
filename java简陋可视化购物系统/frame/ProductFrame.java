// 商品页面
package frame;

import pojo2.Trolley;

import javax.swing.*;
import javax.swing.table.DefaultTableModel;

import pojo1.*;

import java.awt.*;
import java.awt.event.*;
import java.util.ArrayList;

public class ProductFrame extends JFrame implements ActionListener {
    private JTable table;
    private JTextField searchField, idField, quantityField;
    private JButton searchButton, addToCartButton, backButton;
    private DefaultTableModel tableModel;
    User u = null;
    Trolley tro = new Trolley(null);
    ArrayList<Product> productList = new ArrayList<Product>();
    private ArrayList<Product> products = Product.fromFile("txt\\Products.txt");

    public ProductFrame(User u) {
        // 初始化商品列表
        this.u = u;

        // 设置窗口
        setTitle("商品栏");
        setSize(1000, 800);
        setResizable(true); // 不能改变窗口大小
        setLocationRelativeTo(null); // 将窗口定位到屏幕中央
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

        // 创建面板并设置布局
        JPanel panel = new JPanel();
        panel.setLayout(new BoxLayout(panel, BoxLayout.Y_AXIS));
        // 添加表格
        tableModel = new DefaultTableModel(new String[] { "编号", "名称", "类型",
                "数量", "单价" }, 0);
        table = new JTable(tableModel);
        table.getTableHeader().setFont(new Font("宋体", Font.PLAIN, 30));
        table.setFont(new Font("宋体", Font.PLAIN, 30));
        table.setRowHeight(30); // 将行高设置为30像素
        table.setPreferredScrollableViewportSize(new Dimension(1000, 450));

        JScrollPane scrollPane = new JScrollPane(table);
        panel.add(scrollPane);
        // 添加搜索商品的文本框和按钮
        JLabel searchLabel = new JLabel("可通过商品名称或类型进行检索");
        searchLabel.setFont(new Font("宋体", Font.PLAIN, 30));
        panel.add(searchLabel);
        searchField = new JTextField(20);
        searchField.setFont(new Font("宋体", Font.PLAIN, 30));
        panel.add(searchField);

        searchButton = createButton("搜索商品");
        panel.add(searchButton);

        // 添加加入购物车的文本框和按钮
        JLabel idLabel = new JLabel("请输入商品ID");
        idLabel.setFont(new Font("宋体", Font.PLAIN, 30));
        panel.add(idLabel);
        idField = new JTextField(20);
        idField.setFont(new Font("宋体", Font.PLAIN, 30));
        panel.add(idField);

        JLabel quantityLabel = new JLabel("请输入商品数量");
        quantityLabel.setFont(new Font("宋体", Font.PLAIN, 30));
        panel.add(quantityLabel);
        quantityField = new JTextField(20);
        quantityField.setFont(new Font("宋体", Font.PLAIN, 30));
        panel.add(quantityField);

        addToCartButton = createButton("加入购物车");
        panel.add(addToCartButton);

        // 添加返回按钮
        backButton = createButton("返回");
        panel.add(backButton);

        // 将面板添加到窗口中
        add(panel);

        // 显示商品列表
        displayProducts(products);

        // 显示窗口
        setVisible(true);
    }

    private JButton createButton(String text) {
        JButton button = new JButton(text);
        button.setFont(new Font("宋体", Font.PLAIN, 30));
        button.addActionListener(this);
        return button;
    }

    private void displayProducts(ArrayList<Product> products) {
        tableModel.setRowCount(0);
        for (Product product : products) {
            Object[] rowData = new Object[] { product.getId(), product.getName(), product.getCategory(),
                    product.getAmount(), product.getPrice() };
            tableModel.addRow(rowData);
        }
    }

    @Override
    public void actionPerformed(ActionEvent e) {
        if (e.getSource() == searchButton) {
            // 搜索商品
            String keyword = searchField.getText().trim();
            if (keyword.isEmpty()) {
                // 如果搜索关键字为空，则显示所有商品
                displayProducts(products);
            } else {
                // 否则按照名称或类型搜索商品
                ArrayList<Product> result = new ArrayList<>();
                for (Product product : products) {
                    if (product.getName().contains(keyword) || product.getCategory().contains(keyword)) {
                        result.add(product);
                    }
                }
                displayProducts(result);
            }
        } else if (e.getSource() == addToCartButton) {
            // 加入购物车
            String id = idField.getText().trim();
            String quantity = quantityField.getText().trim();
            if (id.isEmpty() || quantity.isEmpty()) {
                JOptionPane.showMessageDialog(this, "请输入商品编号和数量", "提示", JOptionPane.ERROR_MESSAGE);
            } else {
                try {
                    int productId = Integer.parseInt(id);
                    int productQuantity = Integer.parseInt(quantity);
                    Product product = null;
                    for (Product p : products) {
                        if (p.getId() == productId) {
                            product = p;
                            product.setAmount(productQuantity);
                            productList.add(product);
                            break;
                        }
                    }
                    if (product == null) {
                        JOptionPane.showMessageDialog(this, "商品不存在", "提示", JOptionPane.ERROR_MESSAGE);
                    } else if (product.getAmount() < productQuantity) {
                        JOptionPane.showMessageDialog(this, "商品库存不足", "提示", JOptionPane.ERROR_MESSAGE);
                    } else {
                        tro.setProlist(productList);
                        JOptionPane.showMessageDialog(this, "商品已成功添加到购物车", "提示", JOptionPane.INFORMATION_MESSAGE);
                    }
                } catch (NumberFormatException ex) {
                    JOptionPane.showMessageDialog(this, "商品编号和数量必须是数字", "提示", JOptionPane.ERROR_MESSAGE);
                }
            }
        } else if (e.getSource() == backButton) {
            // 返回上一级界面
            dispose();
            new MainFrame(u, tro);
        }
    }
}