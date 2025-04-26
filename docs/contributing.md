# Contributing to CEQUEAU

Thank you for your interest in contributing to CEQUEAU! This document provides guidelines and instructions for contributing to this project.

## Code of Conduct

This project adheres to a Code of Conduct that all contributors are expected to follow. By participating, you are expected to uphold this code.

## How Can I Contribute?

### Reporting Bugs

- **Ensure the bug was not already reported** by searching on GitHub under [Issues](https://github.com/erinconv/CEQUEAU/issues).
- If you're unable to find an open issue addressing the problem, [open a new one](https://github.com/erinconv/CEQUEAU/issues/new). Be sure to include a **title and clear description**, as much relevant information as possible, and a **code sample** or an **executable test case** demonstrating the expected behavior that is not occurring.

### Suggesting Enhancements

- Open a new issue with a clear title and detailed description.
- Provide specific examples and use cases for the enhancement.
- Describe the current behavior and explain what behavior you expected to see instead.

### Pull Requests

1. Fork the repository.
2. Create a new branch (`git checkout -b feature/amazing-feature`).
3. Make your changes.
4. Run tests to ensure your changes don't break existing functionality.
5. Commit your changes (`git commit -m 'Add some amazing feature'`).
6. Push to the branch (`git push origin feature/amazing-feature`).
7. Open a Pull Request.

### Translating the Source Code and User Manual
- We are actively working to translate the source code and user manual from French to English to make it the more more accesible to everyone. If you are fluent in both languages, we would greatly appreciate your help!.
- The original CEQUEAU user manual can be found [here](https://github.com/erinconv/CEQUEAU/blob/main/docs/LEGACY_DOCS/Cequeau.pdf)
- You can contribute by translating code comments, documentation, and any other relevant materials.
- Please open an issue to discuss the specific parts you would like to translate, or feel free to submit a pull request with your translations.
- Ensure that your translations maintain the original meaning and context to help future contributors understand the code and documentation better.

## Development Setup

1. Clone the repository:
   ```
   git clone https://github.com/erinconv/CEQUEAU.git
   cd cequeau
   ```

2. Install dependencies:
   ```
   # Using pip
   pip install -e .
   pip install -r requirements-dev.txt
   ```

3. Set up pre-commit hooks:
   ```
   pre-commit install
   ```

## Coding Standards

### C++ Code Style

- Follow the existing code style in the project.
- Use descriptive variable and function names.
- Comment your code where necessary, especially for complex algorithms.
- Use modern C++ features where appropriate.
- Keep functions small and focused on a single task.
- Avoid global variables.

### Python Code Style

- Follow PEP 8 guidelines.
- Use type hints where possible.
- Document functions and classes using docstrings.
- Write unit tests for new functionality.

## Documentation

- Update documentation for any changes to public APIs.
- Document new features, parameters, and error handling.
- For significant changes, consider updating the examples.

## Testing

- Write unit tests for new functionality.
- Ensure all tests pass before submitting a pull request.
- Consider adding integration tests for larger features.

## Review Process

Pull requests will be reviewed by project maintainers. The review process may include:

- Code review for style, correctness, and performance.
- Testing the changes locally.
- Requesting additional changes or clarification.

## Release Process

The project maintainers will handle releases according to the following process:

1. Update version number in relevant files.
2. Update changelog with notable changes.
3. Create a release tag.
4. Publish the release.

## Additional Resources

- [GitHub Documentation](https://docs.github.com/en)
- [Git Documentation](https://git-scm.com/doc)

Thank you for contributing to CEQUEAU!