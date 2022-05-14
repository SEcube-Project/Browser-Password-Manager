import * as React from "react";
import Button from "@mui/material/Button";
import Dialog from "@mui/material/Dialog";
import DialogActions from "@mui/material/DialogActions";
import DialogContent from "@mui/material/DialogContent";
import DialogContentText from "@mui/material/DialogContentText";
import DialogTitle from "@mui/material/DialogTitle";
import { Tooltip, IconButton } from "@mui/material";
import Delete from "@mui/icons-material/Delete";

export default function AlertDialog() {
  const [popup, setPopup] = React.useState(false);

  const handleClickOpen = () => {
    setPopup(true);
  };

  const handleClose = () => {
    setPopup(false);
  };

  return (
    <Tooltip title="Delete">
      <IconButton color="error" onClick={handleClickOpen}>
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
              Are you sure you want to delete this password?
            </DialogContentText>
          </DialogContent>
          <DialogActions>
            <Button variant="contained" color="success" onClick={handleClose}>
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
  );
}
