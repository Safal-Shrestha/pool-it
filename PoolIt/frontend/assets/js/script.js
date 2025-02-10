// Function to handle form submission
document.getElementById('loginForm').addEventListener('submit', function (event) {
    event.preventDefault(); // Prevent the form from submitting

    // Get input values
    const email = document.querySelector('input[type="email"]').value;
    const password = document.querySelector('input[type="password"]').value;

    // Basic validation
    if (!email || !password) {
        alert('Please fill in all fields.');
        return;
    }

    // Simulate a login request (replace with actual API call)
    console.log('Logging in with:', { email, password });
    alert('Login successful! Redirecting...');
    // Redirect to another page or perform further actions
});

// Function to handle "Sign Up" link click
// document.querySelector('.signup-link').addEventListener('click', function (event) {
//     event.preventDefault(); // Prevent the default link behavior

//     // Simulate a sign-up action
//     console.log('Redirecting to Sign Up page...');
//     alert('Redirecting to Sign Up page...');
//     // Redirect to the sign-up page or open a sign-up modal
// });


document.getElementById('signupForm').addEventListener('submit', function(e) {
    e.preventDefault();
    
    // Get form values
    const name = document.getElementById('name').value;
    const email = document.getElementById('email').value;
    const phone = document.getElementById('phone').value;
    const citizenshipNumber = document.getElementById('citizenshipNumber').value;
    const mobile = document.getElementById('mobile').value;
    const password = document.getElementById('password').value;
    const confirmPassword = document.getElementById('confirmPassword').value;

    // Validation
    if (name && email && phone && citizenshipNumber && mobile && password && confirmPassword) {
        if (password !== confirmPassword) {
            alert('Passwords do not match!');
            return;
        }
        
        // Add your signup API call here
        console.log('Signup attempted with:', { name, email, phone, citizenshipNumber, mobile, password });
        
        // Success animation
        document.querySelector('.container').style.transform = 'translateY(-20px)';
        setTimeout(() => {
            window.location.href = '/dashboard'; // Redirect to dashboard
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
                console.log('Offering ride:', rideDetails);
                // Add code to post ride offering to backend
            } else {
                console.log('Searching rides:', rideDetails);
                // Add code to search for matching rides
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







// settings.js

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
  
  
  
  