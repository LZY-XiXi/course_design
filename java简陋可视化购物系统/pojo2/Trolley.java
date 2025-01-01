package pojo2;

import java.util.*;

import pojo1.Product;

public class Trolley {
    ArrayList<Product> productList;

    public Trolley(ArrayList<Product> productList) {
        this.productList = productList;
    }

    public List<Product> getProlist() {
        return this.productList;
    }

    public void setProlist(ArrayList<Product> productList) {
        this.productList = productList;
    }

    public void toFile(String name) {
        for (Product p : productList) {
            p.toFile(name, false);
        }
    }
}