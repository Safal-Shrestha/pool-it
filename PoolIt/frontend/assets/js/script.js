if (document.getElementById('loginForm')) {
    // Function to handle form submission
    document.getElementById('loginForm').addEventListener('submit', function (event) {
        event.preventDefault(); // Prevent the form from submitting

        const email = document.querySelector('input[type="email"]').value;
        const password = document.querySelector('input[type="password"]').value;

        // Get input values
        const loginDetails = {
            l_email: email,
            l_password: password
        }

        // Basic validation
        if (!email || !password) {
            alert('Please fill in all fields.');
            return;
        }

        fetch("http://localhost:8080/api/login", {
            method: "POST",
            headers: {
                "Content-Type": "application/json"
            },
            body: JSON.stringify(loginDetails)
        })
            .then(response => response.json())
            .then(data => {
                console.log("Success:", data);
            })
            .then(window.location.href = '/home')
            .catch(error => {
                console.error("Error:", error);
            });
    });
}

if(document.getElementById('signupForm'))
{
    document.getElementById('signupForm').addEventListener('submit', function(e) {
        e.preventDefault();
        
        const name = document.getElementById('name').value;
        const email = document.getElementById('email').value;
        const phone = parseInt(document.getElementById('phone').value);
        const citizenshipNumber = document.getElementById('citizenshipNumber').value;
        const password = document.getElementById('password').value;
        const confirmPassword = document.getElementById('confirmPassword').value;
    
        const signupDetails = {
            s_name: name,
            s_email: email,
            s_phone: phone,
            s_citizen: citizenshipNumber,
            s_password: password
        }
    
        // Validation
        if (name && email && phone && citizenshipNumber && password && confirmPassword) {
            if (password !== confirmPassword) {
                alert('Passwords do not match!');
                return;
            }
            
            // Add your signup API call here
            fetch("http://localhost:8080/api/signup", {
                method: "POST",
                headers: {
                    "Content-Type": "application/json"
                },
                body: JSON.stringify(signupDetails)
            })
                .then(response => response.json())
                .then(data => {
                    console.log("Success:", data);
                })
                .catch(error => {
                    console.error("Error:", error);
                });
            
            // Success animation
            document.querySelector('.container').style.transform = 'translateY(-20px)';
            setTimeout(() => {
                window.location.href = '/home';
            }, 1000);
        } else {
            // Error animation
            const container = document.querySelector('.container');
            container.style.animation = 'shake 0.5s';
            setTimeout(() => {
                container.style.animation = '';
            }, 500);
        }
    });

    // Add shake animation for errors
    const style = document.createElement('style');
    style.textContent = `
        @keyframes shake {
            0%, 100% { transform: translateX(0); }
            25% { transform: translateX(-10px); }
            75% { transform: translateX(10px); }
        }
    `;
    document.head.appendChild(style);

    // Add input interactions
    document.querySelectorAll('.form-group input').forEach(input => {
        input.addEventListener('focus', () => {
            input.parentElement.style.transform = 'scale(1.02)';
        });
        
        input.addEventListener('blur', () => {
            input.parentElement.style.transform = 'scale(1)';
        });
    });
}

const map = L.map('map').setView([51.505, -0.09], 13);
        L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png').addTo(map);

        // Role switching functionality
        let isDriver = true;
        const roleSwitch = document.getElementById('roleSwitch');
        const seatsGroup = document.getElementById('seatsGroup');
        const submitBtn = document.getElementById('submitBtn');

        roleSwitch.addEventListener('click', () => {
            isDriver = !isDriver;
            roleSwitch.textContent = isDriver ? 'Driver' : 'Passenger';
            roleSwitch.style.background = isDriver ? '#4CAF50' : '#1a237e';
            seatsGroup.style.display = isDriver ? 'block' : 'none';
            submitBtn.textContent = isDriver ? 'Offer Ride' : 'Find Rides';
        });

        // Hamburger menu toggle
        document.querySelector('.hamburger').addEventListener('click', () => {
            document.querySelector('.nav-links').classList.toggle('active');
        });

        // Form submission
        document.getElementById('rideForm').addEventListener('submit', (e) => {
            e.preventDefault();
            const rideDetails = {
                from: document.getElementById('from').value,
                to: document.getElementById('to').value,
                time: document.getElementById('time').value,
                seats: isDriver ? document.getElementById('seats').value : null,
                role: isDriver ? 'driver' : 'passenger'
            };

            if (isDriver) {
                fetch("http://localhost:8080/api/rideUpdate", {
                    method: "POST",
                    headers: {
                        "Content-Type": "application/json"
                    },
                    body: JSON.stringify(data)
                })
                    .then(response => response.json())
                    .then(rideDetails => {
                        console.log("Success:", rideDetails);
                    })
                    .catch(error => {
                        console.error("Error:", error);
                    });
            } else {
                fetch("http://localhost:8080/api/rideFetch", {
                    method: "POST",
                    headers: {
                        "Content-Type": "application/json"
                    },
                    body: JSON.stringify(data)
                })
                    .then(response => response.json())
                    .then(rideDetails => {
                        console.log("Success:", rideDetails);
                    })
                    .catch(error => {
                        console.error("Error:", error);
                    });
            }
        });

        // Close menu when clicking outside
        document.addEventListener('click', (e) => {
            if (!e.target.closest('.hamburger') && !e.target.closest('.nav-links')) {
                document.querySelector('.nav-links').classList.remove('active');
            }
        });

        // Initialize map markers (example)
        L.marker([51.5, -0.09]).addTo(map)
            .bindPopup('Available Ride<br>To: Central London')
            .openPopup();

// Toggle dropdown visibility when settings button is clicked
document.getElementById("settingsButton").addEventListener("click", function(e) {
    e.stopPropagation(); // Prevent click event from propagating
    const dropdown = document.querySelector(".dropdown");
    dropdown.classList.toggle("show");
  });
  
  // Close dropdown if clicked outside
  document.addEventListener("click", function(e) {
    const dropdown = document.querySelector(".dropdown");
    if (!dropdown.contains(e.target)) {
      dropdown.classList.remove("show");
    }
  });
  
  
  
  