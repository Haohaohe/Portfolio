/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package Integracion.Eventos;

import Integracion.Conectar;
import Negocio.Eventos.TEvento;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.Arrays;

/**
 *
 * @author HAO-H
 */
public class DAOEvento {
    
    public ArrayList<TEvento> Listar_Eventos(){
        ArrayList<TEvento> list = new ArrayList<>();
        Conectar conec = new Conectar();
        String sql = "SELECT * FROM evento;";
        ResultSet rs = null;
        PreparedStatement ps = null;
        try{
            ps = conec.getConnection().prepareStatement(sql);
            rs = ps.executeQuery();
            String aux;
            String[] listString;
            while(rs.next()){
                TEvento ev = new TEvento();
                ArrayList<String> auxx = new ArrayList<>();
                ev.setNombre(rs.getString(1));
                
                aux = rs.getString(2);
                listString = aux.split(",");
                auxx.addAll(Arrays.asList(listString));
                ev.setListaUsuarios(auxx);
                
                auxx = new ArrayList<>();
                aux = rs.getString(3);
                listString = aux.split(",");
                auxx.addAll(Arrays.asList(listString));
                ev.setListapromos(auxx);
                
                list.add(ev);
            }
            
        }catch(SQLException ex){
            System.out.println(ex.getMessage());
        }catch(Exception ex){
            System.out.println(ex.getMessage());
        }finally{
            try{
                ps.close();
                rs.close();
                conec.desconectar();
            }catch(Exception ex){}
        }
        return list;
    }
    
    public void Agregar_Evento(TEvento ev){
    Conectar conec = new Conectar();
    String sql = "INSERT INTO evento (nombre, usuarios, promociones) VALUES (?, ?, ?)";
    String aux = "";
    PreparedStatement ps = null;
    try{
        ps = conec.getConnection().prepareStatement(sql);
        ps.setString(1, ev.getNombre());
        
        if(ev.getListaUsuarios() != null){
            for(String l: ev.getListaUsuarios()){
                aux = aux + l + ",";
            }
            ps.setString(2, aux.substring(0, aux.length()-1));
        }
        else ps.setString(2, "");

        if(ev.getListapromos() != null){
            aux = "";
            for(String l: ev.getListapromos()){
                aux = aux + l + ",";
            }
            ps.setString(3, aux.substring(0, aux.length()-1));
        }
        else ps.setString(3, "");
        ps.executeUpdate();
    }catch(SQLException ex){
        System.out.println(ex.getMessage());
    }catch(Exception ex){
        System.out.println(ex.getMessage());
    }finally{
        try{
            ps.close();
            conec.desconectar();
        }catch(Exception ex){}
    }
}

    public void Modificar_Evento(TEvento ev){
    Conectar conec = new Conectar();
    String aux = "";
    String sql = "UPDATE evento SET usuarios = ?, promociones = ? WHERE nombre = ?;";
    PreparedStatement ps = null;
    try{
        ps = conec.getConnection().prepareStatement(sql);

        ps.setString(3, ev.getNombre());

        for(String l: ev.getListaUsuarios()){
            aux = aux + l + ",";
        }
        ps.setString(1, aux.substring(0, aux.length()-1));

        aux = "";
        for(String l: ev.getListapromos()){
            aux = aux + l + ",";
        }
        ps.setString(2, aux.substring(0, aux.length()-1));

        ps.executeUpdate();
    }catch(SQLException ex){
        ex.printStackTrace();
    }finally{
        try{
            ps.close();
            conec.desconectar();
        }catch(Exception ex){}
    }
}

    public void Eliminar_Evento(TEvento ev){
    Conectar conec = new Conectar();
    String sql = "DELETE FROM evento WHERE nombre = ?;";
    PreparedStatement ps = null;
    try{
        ps = conec.getConnection().prepareStatement(sql);
        ps.setString(1, ev.getNombre());

        ps.executeUpdate();
    }catch(SQLException ex){
        System.out.println(ex.getMessage());
    }catch(Exception ex){
        System.out.println(ex.getMessage());
    }finally{
        try{
            ps.close();
            conec.desconectar();
        }catch(Exception ex){}
    }
}
    
}
