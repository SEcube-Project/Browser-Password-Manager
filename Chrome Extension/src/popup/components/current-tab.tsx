import * as React from "react";
import Box from "@mui/material/Box";
import { styled, ThemeProvider, createTheme } from "@mui/material/styles";
import List from "@mui/material/List";
import ListItem from "@mui/material/ListItem";
import ListItemButton from "@mui/material/ListItemButton";
import ListItemIcon from "@mui/material/ListItemIcon";
import ListItemText from "@mui/material/ListItemText";
import Paper from "@mui/material/Paper";
import KeyboardArrowDown from "@mui/icons-material/KeyboardArrowDown";
import {
  Alert,
  Button,
  Dialog,
  DialogActions,
  DialogContent,
  DialogContentText,
  DialogTitle,
  IconButton,
  ListSubheader,
  Tooltip,
} from "@mui/material";
import AccountCircle from "@mui/icons-material/AccountCircle";
import Password from "@mui/icons-material/Password";
import Link from "@mui/icons-material/Link";
import Edit from "@mui/icons-material/Edit";
import ContentCopy from "@mui/icons-material/ContentCopy";
import AlertDialog from "./delete-dialog";
import Delete from "@mui/icons-material/Delete";

var data = [
  { icon: <AccountCircle />, label: "Username" },
  { icon: <Password />, label: "Password" },
  { icon: <Link />, label: "URL" },
];

const FireNav = styled(List)<{ component?: React.ElementType }>({
  "& .MuiListItemButton-root": {
    paddingLeft: 10,
    paddingRight: 5,
  },
  "& .MuiListItemIcon-root": {
    minWidth: 0,
    marginRight: 16,
  },
  "& .MuiSvgIcon-root": {
    fontSize: 20,
  },
});

export default function CustomizedList() {
  const [open, setOpen] = React.useState([false]);
  const [popup, setPopup] = React.useState(false);

  const handleClickOpen = () => {
    setPopup(true);
  };

  const handleClose = () => {
    setPopup(false);
  };

  const handleClick = (index: number, item, section) => {
    console.log("index", index, "item", item, "section", section);
    setOpen((prevOpen) => {
      const newOpen = [...prevOpen];
      newOpen[index] = !newOpen[index];
      console.log("newOpen", newOpen);
      return newOpen;
    });
  };

  var letters = [
    "a",
    "b",
    "c",
    "d",
    "e",
    // "f",
    // "g",
    // "h",
    // "i",
    // "j",
    // "k",
    // "l",
    // "m",
    // "n",
    // "o",
    // "p",
    // "q",
    // "r",
    // "s",
    // "t",
    // "u",
    // "v",
    // "w",
    // "x",
    // "y",
    // "z",
  ];

  var numbers = [
    0, 1,
    // 2,
    // 3,
    // 4,
  ];

  return (
    <Box>
      <ThemeProvider
        theme={createTheme({
          components: {
            MuiListItemButton: {
              defaultProps: {
                disableTouchRipple: true,
              },
            },
          },
          palette: {
            primary: { main: "rgba(25, 118, 210, 0.3)" },
            background: { paper: "rgba(25, 118, 210, 0.3)" },
          },
        })}
      >
        <Paper elevation={0} sx={{ maxWidth: "100%" }}>
          <FireNav component="nav" disablePadding>
            <List
              sx={{
                width: "100%",
                height: "380px",
                maxWidth: "100%",
                position: "relative",
                overflow: "auto",
                "& ul": { padding: 0 },
              }}
              subheader={<li />}
            >
              {letters.map((sectionId) => (
                <li key={`section-${sectionId}`}>
                  <ul>
                    <ListSubheader
                      sx={{ bgcolor: "rgba(10, 50, 105, 1)", color: "white" }}
                    >
                      {sectionId.toUpperCase()}
                    </ListSubheader>

                    {numbers.map((item) => (
                      <ListItem key={`item-${sectionId}-${item}`}>
                        <Box
                          sx={{
                            bgcolor: open[
                              item + convertToIndex(sectionId) * numbers.length
                            ]
                              ? "rgba(25, 118, 210, 0.7)"
                              : "rgba(25, 118, 210, 0.7)",
                            width: "360px",
                          }}
                        >
                          {console.log("item", item)}
                          {console.log("sectionId", sectionId)}
                          {console.log(
                            "index",
                            item + convertToIndex(sectionId) * numbers.length
                          )}
                          <ListItemButton
                            onClick={() =>
                              handleClick(
                                item +
                                  convertToIndex(sectionId) * numbers.length,
                                item,
                                sectionId
                              )
                            }
                          >
                            <ListItemText
                              primary="Hostname"
                              primaryTypographyProps={{
                                noWrap: true,
                                fontSize: 15,
                                fontWeight: "medium",
                                lineHeight: "20px",
                                mb: "2px",
                              }}
                              secondary="Username, Password, URL"
                              secondaryTypographyProps={{
                                noWrap: true,
                                fontSize: 12,
                                lineHeight: "16px",
                                color: open[
                                  item +
                                    convertToIndex(sectionId) * numbers.length
                                ]
                                  ? "rgba(0, 0, 0, 1)"
                                  : "rgba(0, 0, 0, 1)",
                              }}
                              sx={{ my: 0 }}
                            />
                            <Tooltip title="Delete">
                              <IconButton
                                color="error"
                                onClick={handleClickOpen}
                              >
                                <Delete />
                                <Dialog
                                  open={popup}
                                  onClose={handleClose}
                                  aria-labelledby="alert-dialog-title"
                                  aria-describedby="alert-dialog-description"
                                >
                                  <DialogTitle id="alert-dialog-title">
                                    {"Delete password?"}
                                  </DialogTitle>
                                  <DialogContent>
                                    <DialogContentText id="alert-dialog-description">
                                      Are you sure you want to delete this
                                      password?
                                    </DialogContentText>
                                  </DialogContent>
                                  <DialogActions>
                                    <Button
                                      variant="contained"
                                      color="success"
                                      onClick={handleClose}
                                    >
                                      Keep
                                    </Button>
                                    <Button
                                      variant="contained"
                                      color="error"
                                      onClick={handleClose}
                                      autoFocus
                                    >
                                      Delete
                                    </Button>
                                  </DialogActions>
                                </Dialog>
                              </IconButton>
                            </Tooltip>
                            <Tooltip
                              title={
                                open[
                                  item +
                                    convertToIndex(sectionId) * numbers.length
                                ]
                                  ? "Collapse"
                                  : "Expand"
                              }
                            >
                              <IconButton size="small">
                                <KeyboardArrowDown
                                  sx={{
                                    transform: open[
                                      item +
                                        convertToIndex(sectionId) *
                                          numbers.length
                                    ]
                                      ? "rotate(-180deg)"
                                      : "rotate(0)",
                                    transition: "0.2s",
                                  }}
                                />
                              </IconButton>
                            </Tooltip>
                          </ListItemButton>
                          {open[
                            item + convertToIndex(sectionId) * numbers.length
                          ] &&
                            data.map((item) => (
                              <ListItemButton
                                key={item.label}
                                sx={{
                                  py: 0,
                                  minHeight: 32,
                                  color: "rgba(0, 0, 0, 1)",
                                }}
                              >
                                <ListItemIcon sx={{ color: "inherit" }}>
                                  {item.icon}
                                </ListItemIcon>
                                <ListItemText
                                  primary={item.label}
                                  primaryTypographyProps={{
                                    fontSize: 14,
                                    fontWeight: "medium",
                                  }}
                                />
                                <Tooltip title="Edit">
                                  <IconButton size="small">
                                    <Edit />
                                  </IconButton>
                                </Tooltip>
                                <Tooltip title="Copy">
                                  <IconButton size="small">
                                    <ContentCopy />
                                  </IconButton>
                                </Tooltip>
                              </ListItemButton>
                            ))}
                        </Box>
                      </ListItem>
                    ))}
                  </ul>
                </li>
              ))}
            </List>
          </FireNav>
        </Paper>
      </ThemeProvider>
    </Box>
  );
}

/**
 * Convert A => 0, B => 1, C => 2, D => 3, E => 4 etc..
 * @param char The letter to convert
 * @returns The number of the letter
 */
const convertToIndex = (char: string) => {
  return char.toUpperCase().charCodeAt(0) - 65;
};
