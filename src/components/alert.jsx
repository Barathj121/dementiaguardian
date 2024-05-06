import React, { useEffect, useState } from 'react';
import axios from 'axios';
import './alert.css'; // Import the CSS file for styling
import { MapContainer, TileLayer, Marker, Popup } from 'react-leaflet';
import 'leaflet/dist/leaflet.css';

const AlertMonitor = () => {
  const [latestSerialNumber, setLatestSerialNumber] = useState(null);

  const fetchLatestSerialNumber = async () => {
    try {
      const response = await axios.get('http://localhost:8000/alert-history');
      const latestAlert = response.data[response.data.length - 1];
      setLatestSerialNumber(latestAlert.serial_number);
    } catch (error) {
      console.error('Error fetching latest serial number:', error);
    }
  };

  useEffect(() => {
    fetchLatestSerialNumber();
  }, []);

  useEffect(() => {
    const interval = setInterval(fetchLatestSerialNumber, 5000); // Fetch latest serial number every 5 seconds
    return () => clearInterval(interval);
  }, []);

  return (
    <div className="container">
      <div className="header">
        <h1>Dementia Guardian</h1>
        <NavBar />
      </div>
      {latestSerialNumber && (
        <div className="alert-container">
          <h2>Latest Serial Number: {latestSerialNumber}</h2>
          <hr />
          <AlertChecker latestSerialNumber={latestSerialNumber} />
        </div>
      )}
    </div>
  );
};

const NavBar = () => {
  return (
    <nav>
      <ul className="nav-links">
        <li>
          <a href="#" className="nav-link">Alert Page</a>
        </li>
        <li>
          <a href="#" className="nav-link">View Alert History</a>
        </li>
      </ul>
    </nav>
  );
};

const AlertChecker = ({ latestSerialNumber }) => {
  const [alert, setAlert] = useState(null);

  useEffect(() => {
    const checkAlert = async () => {
      try {
        const response = await axios.get('http://localhost:8000/alert-history');
        const latestAlert = response.data[response.data.length - 1];
        if (latestAlert.serial_number !== latestSerialNumber) {
          setAlert(latestAlert);
          setTimeout(() => {
            setAlert(null); // Hide alert after 1 minute
          }, 60000);
        }
      } catch (error) {
        console.error('Error checking alert:', error);
      }
    };

    const interval = setInterval(checkAlert, 5000); // Check for alert every 5 seconds
    return () => clearInterval(interval);
  }, [latestSerialNumber]);

  return (
    <div>
      {alert && (
        <div className="alert-box">
          <h4>Dementia Guardian</h4>
          <p>The patient has pressed the alert button</p>
          <hr />
          <div style={{ height: '200px', width: '100%' }}>
            <MapContainer center={[alert.location_lat, alert.location_long]} zoom={15} style={{ height: '100%', width: '100%' }}>
              <TileLayer url="https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png" />
              <Marker position={[alert.location_lat, alert.location_long]}>
                <Popup>
                  A pretty CSS3 popup. <br /> Easily customizable.
                </Popup>
              </Marker>
            </MapContainer>
          </div>
          <p>Mode: {alert.source}</p>
          {formatTimestamp(alert.time_stamp)}
          <button onClick={() => setAlert(null)}>Close Alert</button>
        </div>
      )}
    </div>
  );
};

const formatTimestamp = (timestamp) => {
  const dateObject = new Date(timestamp);
  const date = dateObject.toLocaleDateString();
  const time = dateObject.toLocaleTimeString();

  return (
    <div>
      <p>Date: {date}</p>
      <p>Time: {time}</p>
    </div>
  );
};

export default AlertMonitor;
