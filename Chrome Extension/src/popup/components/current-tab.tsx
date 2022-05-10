import { List, ListItem, ListItemText } from "@mui/material";
import React from "react";

export default function ListPasswordElements(props) {
    return (
      <List sx={{ width: 400, height: 400 }}>
        {props.passwordData?.map(({ username, password, url }, index) => (
          <ListItem button key={index + url} alignItems="flex-start">
            <ListItemText primary={username} />
            <ListItemText primary={password} />
            <ListItemText primary={url} />
          </ListItem>
        ))}
      </List>
    );
  }