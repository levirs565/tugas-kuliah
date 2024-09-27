const form = $("#form");
const table = $("#table");
const namaInput = $("#namaInput");
const hargaInput = $("#hargaInput");
const hargaError = $("#hargaError");
const btnSubmit = $("#btnSubmit");
const modal = new bootstrap.Modal($("#errorDialog"));
const errorText = $("#errorText");
const tableBody = $("#tableBody");
const rowTemplate = $("#rowTemplate");

let editIndex = -1;

const items = localStorage.getItem("items") == null ? [
    {
        nama: "Nasi Goreng",
        harga: 25000
    },
    {
        nama: "Ayam Geprek 1",
        harga: 23000
    },
    {
        nama: "Ikan Bakar Nilai",
        harga: 3000
    }
] : JSON.parse(localStorage.getItem("items"));

function save() {
    localStorage.setItem("items", JSON.stringify(items));
}

function setEditIndex(index) {
    editIndex = index;

    if (editIndex == -1) {
        btnSubmit.val("Add");
        namaInput.val("");
        hargaInput.val("");
        table.css("display", "");
        renderItems();
    } else {
        btnSubmit.val("Edit");
        tableBody.html("");
        table.css("display", "none");

        const item = items[index];

        namaInput.val(item.nama);
        hargaInput.val(formatNumber(item.harga));
    }
}

function onItemDelete(e) {
    items.splice(e.data.index, 1);
    save();
    renderItems();
}

function onItemEdit(e) {
    setEditIndex(e.data.index);
}

function formatNumber(num) {
    const thousand = [];
    const value = String(num);
    for (let i = value.length; i > 0; i -= 3) {
        thousand.unshift(value.substring(i - 3, i));
    }

    return thousand.join(".");
}

function cleanNumber(num) {
    return String(num).replaceAll(".", "");
}

function renderItems() {
    tableBody.html("");

    let i = 0;
    for (const item of items) {
        const element = rowTemplate.contents().clone();

        element.find("[data-no]").text(String(i + 1));
        element.find("[data-nama]").text(item.nama);
        element.find("[data-harga]").text(formatNumber(item.harga));

        const eventData = {
            index: i
        }
        element.find("[data-delete]").on("click", eventData, onItemDelete);
        element.find("[data-edit]").on("click", eventData, onItemEdit)

        tableBody.append(element);

        i++;;
    }
}

renderItems();

function onHargaInput(e) {
    let error = "";
    const value = cleanNumber(hargaInput.val());

    if (value.length == 0) {
        error = "Harga tidak boleh kosong";
    } else if (!$.isNumeric(value)) {
        error = "Harga hanya boleh berisi angka";
    }

    hargaError.text(error);
    hargaInput.get(0).setCustomValidity(error);

    if (error != "") return;

    const rawInput = hargaInput.val();
    let digitBeforeCursor = 0;
    const oldSelection = hargaInput.get(0).selectionStart;
    for (let i = 0; i < oldSelection; i++) {
        if ($.isNumeric(rawInput[i])) {
            digitBeforeCursor++;
        }
    }

    const newValue = formatNumber(value);
    hargaInput.val(newValue);

    let newSelection = 0;
    for (let currentDigit = 0; currentDigit < digitBeforeCursor; newSelection++) {
        if ($.isNumeric(newValue[newSelection])) {
            currentDigit++;
        }
    }

    hargaInput.get(0).setSelectionRange(newSelection, newSelection);
}

hargaInput.on("input", onHargaInput);

function onSubmitClick(e) {
    e.preventDefault();

    form.addClass("was-validated")

    const messages = [];

    if (namaInput.val().length < 6) {
        messages.push("Nama kurang dari 6 karakter");
    }

    if (!$.isNumeric(hargaInput.val())) {
        messages.push("Harga bukan angka");
    }

    if (messages.length > 0) {
        errorText.text(messages.join(". "));
        modal.show();
    } else {
        const harga = parseInt(cleanNumber(hargaInput.val()));
        if (editIndex == -1) {
            items.push({
                nama: namaInput.val(),
                harga: harga
            });
        } else {
            items[editIndex] = {
                nama: namaInput.val(),
                harga: harga
            };
        }
        save();

        setEditIndex(-1);
        form.removeClass("was-validated")
    }
}

btnSubmit.on("click", onSubmitClick);