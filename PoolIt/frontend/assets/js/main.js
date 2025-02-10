document.addEventListener("DOMContentLoaded", function () {
    // Initialize map (Updated to show Nepal)
    const map = L.map("map").setView([27.7172, 85.3240], 13); // Coordinates for Kathmandu, Nepal
    L.tileLayer("https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png").addTo(map);

    // Role switching functionality
    let isDriver = true;
    const roleSwitch = document.getElementById("roleSwitch");
    const seatsGroup = document.getElementById("seatsGroup");
    const submitBtn = document.getElementById("submitBtn");

    roleSwitch.addEventListener("click", () => {
      isDriver = !isDriver;
      roleSwitch.textContent = isDriver ? "Driver" : "Passenger";
      roleSwitch.style.background = isDriver ? "#4CAF50" : "#1a237e";
      seatsGroup.style.display = isDriver ? "block" : "none";
      submitBtn.textContent = isDriver ? "Offer Ride" : "Find Rides";
    });

    // Set default time to current date with only time selection
    const timeInput = document.getElementById("time");
    const now = new Date();
    const formattedTime = now.toISOString().slice(11, 16); // Get HH:MM format
    timeInput.value = formattedTime;
    timeInput.setAttribute("min", formattedTime);

    // Hamburger menu toggle
    document.querySelector(".hamburger").addEventListener("click", () => {
      document.querySelector(".nav-links").classList.toggle("active");
    });

    // Close menu when clicking outside
    document.addEventListener("click", (e) => {
      if (!e.target.closest(".hamburger") && !e.target.closest(".nav-links")) {
        document.querySelector(".nav-links").classList.remove("active");
      }
    });

    // Form submission
    document.getElementById("rideForm").addEventListener("submit", (e) => {
      e.preventDefault();
      const rideDetails = {
        from: document.getElementById("from").value,
        to: document.getElementById("to").value,
        time: timeInput.value,
        seats: isDriver ? document.getElementById("seats").value : null,
        role: isDriver ? "driver" : "passenger"
      };

      if (isDriver) {
        console.log("Offering ride:", rideDetails);
        // Add code to post ride offering to backend
      } else {
        console.log("Searching rides:", rideDetails);
        // Add code to search for matching rides
      }
    });

    // Initialize map markers (example)
    L.marker([27.7172, 85.3240]).addTo(map)
      .bindPopup("Available Ride<br>To: Kathmandu")
      .openPopup();
  });