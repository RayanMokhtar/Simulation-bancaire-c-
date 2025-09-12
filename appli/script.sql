CREATE TABLE IF NOT EXISTS simulations (
    id INT AUTO_INCREMENT PRIMARY KEY,
    timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    duration INT NOT NULL,  -- Paramètre
    num_cashiers INT NOT NULL,  -- Paramètre
    min_service_time INT NOT NULL,  -- Paramètre
    max_service_time INT NOT NULL,  -- Paramètre
    client_arrival_interval INT NOT NULL,  -- Paramètre
    priority_client_rate DOUBLE NOT NULL,  -- Paramètre
    client_patience_time INT NOT NULL,  -- Paramètre
    served_clients INT NOT NULL,  -- Stat
    non_served_clients INT NOT NULL,  -- Stat
    satisfaction_rate DOUBLE NOT NULL,  -- Stat
    avg_waiting_time DOUBLE NOT NULL,  -- Stat
    avg_service_time DOUBLE NOT NULL  -- Stat
);

CREATE TABLE IF NOT EXISTS cashier_occupancy (
    id INT AUTO_INCREMENT PRIMARY KEY,
    simulation_id INT NOT NULL,
    cashier_id INT NOT NULL,  -- ID du caissier (0, 1, 2...)
    occupation_rate DOUBLE NOT NULL,  -- Taux d'occupation pour ce caissier
    FOREIGN KEY (simulation_id) REFERENCES simulations(id)
);


-- jeux de données de tests ... pour tester l'insertion des tables 
INSERT INTO simulations (duration, num_cashiers, served_clients, non_served_clients, satisfaction_rate, avg_waiting_time, avg_service_time) 
VALUES (100, 3, 80, 20, 80.0, 5.5, 10.2);


INSERT INTO cashier_occupancy (simulation_id, num_cashiers, avg_occupation_rate) 
VALUES (1, 3, 75.5);